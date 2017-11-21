#include <vector>
#include <string>
#include <map>
#include <assert.h>

extern "C" {
#	include <lua.h>
}

enum ValueType
{
	VT_Unknown,

	VT_Bool,
	VT_Int,
	VT_String,
	VT_Path
};

class ValueDef
{
public:
	std::string name;
	std::vector<std::string> scope; // deprecate this?
	ValueType type = VT_Unknown;
	bool isList = false;
	bool isKeyed = false;
	bool tokens = false;
	bool pathVars = false;
	bool allowDuplicates = false;
	std::vector<std::string> allowed;
};

class Value
{
public:
	Value(ValueDef *def, bool val) : def(def), val(val)						{ assert(def->type == VT_Bool); }
	Value(ValueDef *def, std::string val) : def(def), val(std::move(val))	{ assert(def->type == VT_String || def->type == VT_Path); }
	Value(ValueDef *def, std::vector<std::string> vals) : def(def)
	{
		assert(def->isList && def->type == VT_String);
		for (auto &v : vals)
			list.push_back(std::move(v));
	}

	struct V
	{
		V() : f(0.0) {}
		V(const V &v) : f(v.f), s(v.s) {}
		V(V &&v) : f(v.f), s(std::move(v.s)) {}

		V(bool b) : b(b) {}
		V(std::string str) : f(0.0), s(std::move(str)) {}

		union
		{
			bool b;
			int i;
			double f;
		};
		std::string s;
	};

	ValueDef *def;
	V val;
	std::vector<V> list;
};

class Filter
{
public:
	std::vector<std::string> criteria;
};

class Block
{
public:
	Filter filter;

	std::map<std::string, Value> values;

	std::vector<Block*> children;
};


std::map<std::string, ValueDef*> apis;
Block *rootBlock;
Block *currentBlock;



std::string getString(lua_State *L, bool pop = true)
{
	// TODO: better strategy?
	assert(lua_isstring(L, -1));

	std::string value = lua_tostring(L, -1);

	if (pop)
		lua_pop(L, 1);

	return value;
}
std::vector<std::string> getStringList(lua_State *L, bool pop = true)
{
	std::vector<std::string> l;

	if (lua_istable(L, -1))
	{
		int i = 1;
		do
		{
			lua_pushnumber(L, i++);
			lua_gettable(L, -2);
			if (lua_isstring(L, -1))
				l.push_back(lua_tostring(L, -1));
			else if (lua_isnil(L, -1))
				i = 0;
			lua_pop(L, 2);
		} while (i);
	}
	else if (lua_isstring(L, -1))
		l.push_back(lua_tostring(L, -1));

	if (pop)
		lua_pop(L, 1);

	return l;
}
bool getBool(lua_State *L, bool pop = true)
{
	bool value = false;

	if (lua_isboolean(L, -1))
		value = !!lua_toboolean(L, -1);
	else if (lua_isstring(L, -1))
	{
		const char *s = lua_tostring(L, -1);
		if (!stricmp(s, "on") || !stricmp(s, "true") || !stricmp(s, "yes"))
			value = true;
		else if (stricmp(s, "off") && stricmp(s, "false") && stricmp(s, "no"))
			assert(false); // unexpected value!
	}
	else
		assert(false); // unexpected type?!

	if (pop)
		lua_pop(L, 1);

	return value;
}



int n_register(lua_State *L)
{
	ValueDef *def = new ValueDef;

	lua_pushstring(L, "name");
	lua_gettable(L, -2);
	if (lua_isstring(L, -1))
		def->name = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "scope");
	lua_gettable(L, -2);
	if (lua_isstring(L, -1))
		def->scope.push_back(lua_tostring(L, -1));
	else if (lua_istable(L, -1))
	{
		int i = 1;
		do
		{
			lua_pushnumber(L, i++);
			lua_gettable(L, -2);
			if (lua_isstring(L, -1))
				def->scope.push_back(lua_tostring(L, -1));
			else if (lua_isnil(L, -1))
				i = 0;
			lua_pop(L, 2);
		} while (i);
	}
	lua_pop(L, 1);

	lua_pushstring(L, "kind");
	lua_gettable(L, -2);
	if (lua_isstring(L, -1))
	{
		const char *kind = nullptr;
		kind = lua_tostring(L, -1);
		if (!strncmp(kind, "list:", 5))
		{
			def->isList = true;
			kind += 5;
		}
		else if (!strncmp(kind, "keyed:", 6))
		{
			def->isKeyed = true;
			kind += 6;
		}
		if (!strcmp(kind, "boolean"))
			def->type = VT_Bool;
		else if (!strcmp(kind, "string"))
			def->type = VT_String;
		else if (!strcmp(kind, "path"))
			def->type = VT_Path;
	}
	lua_pop(L, 1);

	lua_pushstring(L, "allowed");
	lua_gettable(L, -2);
	if (lua_istable(L, -1))
	{
		int i = 1;
		do
		{
			lua_pushnumber(L, i++);
			lua_gettable(L, -2);
			if (lua_isstring(L, -1))
				def->allowed.push_back(lua_tostring(L, -1));
			else if (lua_isnil(L, -1))
				i = 0;
			lua_pop(L, 2);
		} while (i);
	}
	lua_pop(L, 1);

	lua_pushstring(L, "tokens");
	lua_gettable(L, -2);
	if (lua_isboolean(L, -1))
		def->tokens = !!lua_toboolean(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "pathVars");
	lua_gettable(L, -2);
	if (lua_isboolean(L, -1))
		def->pathVars = !!lua_toboolean(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "allowDuplicates");
	lua_gettable(L, -2);
	if (lua_isboolean(L, -1))
		def->allowDuplicates = !!lua_toboolean(L, -1);
	lua_pop(L, 1);

	apis.insert(std::make_pair(def->name, def));

	return 0;
}

int n_add(lua_State *L)
{
	assert(lua_isstring(L, -2));

	std::string name = lua_tostring(L, -2);

	ValueDef *def = apis.at(name);
	if (!def)
	{
		// no api?!
		assert(def);
	}

	switch (def->type)
	{
		case VT_String:
		{
			if (def->isList)
				currentBlock->values.emplace(std::move(name), Value{ def, getStringList(L) });
			else
				currentBlock->values.emplace(std::move(name), Value{ def, getString(L) });
		}
		break;

		case VT_Path:
		{
			if (def->isList)
				currentBlock->values.emplace(std::move(name), Value{ def, getStringList(L) });
			else
				currentBlock->values.emplace(std::move(name), Value{ def, getString(L) });
		}
		break;

		case VT_Bool:
		{
			if (def->isList)
			{

			}
			else
				currentBlock->values.emplace(std::move(name), Value{ def, getBool(L) });
		}
		break;

		default:
			// unexpected type?
			break;
	}

	lua_pop(L, 2);

	return 0;
}

int n_remove(lua_State *L)
{
	L;
	return 0;
}

int n_initNativeApi(lua_State *L)
{
	lua_getglobal(L, "premake");
	lua_pushstring(L, "api");
	lua_gettable(L, -2);

	// 'api' table is at top of stack...
	lua_pushstring(L, "n_register");
	lua_pushcfunction(L, n_register);
	lua_settable(L, -3);

	lua_pushstring(L, "n_add");
	lua_pushcfunction(L, n_add);
	lua_settable(L, -3);

	lua_pushstring(L, "n_remove");
	lua_pushcfunction(L, n_remove);
	lua_settable(L, -3);

	lua_pop(L, 2); // pop 'api', 'premake'

	return 0;
}

extern "C" void initNative(lua_State *L)
{
	rootBlock = new Block;
	currentBlock = rootBlock;

	lua_getglobal(L, "premake");

	lua_pushstring(L, "n_initNativeApi");
	lua_pushcfunction(L, &n_initNativeApi);
	lua_settable(L, -3);

	lua_pop(L, 1); // pop 'premake'
}
