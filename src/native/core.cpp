#include "core.h"
#include "block.h"
#include "api.h"


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
			lua_pop(L, 1);
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


int n_global(lua_State *L)
{
	L;
}

int n_workspace(lua_State *L)
{
	L;
}

int n_solution(lua_State *L)
{
	L;
}

int n_project(lua_State *L)
{
	L;
}

int n_group(lua_State *L)
{
	L;
}

int n_configuration(lua_State *L)
{
	L;
}

int n_filter(lua_State *L)
{
	L;
}

int n_pushFilter(lua_State *L)
{
	L;
}

int n_popFilter(lua_State *L)
{
	L;
}

int n_registerApi(lua_State *L)
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
			lua_pop(L, 1);
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

	addApi(def);

	return 0;
}

int n_addOption(lua_State *L)
{
	assert(lua_isstring(L, -2));

	std::string name = lua_tostring(L, -2);

	ValueDef *def = getApi(name);
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

int n_removeOption(lua_State *L)
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
	lua_pushcfunction(L, n_registerApi);
	lua_settable(L, -3);

	lua_pushstring(L, "n_add");
	lua_pushcfunction(L, n_addOption);
	lua_settable(L, -3);

	lua_pushstring(L, "n_remove");
	lua_pushcfunction(L, n_removeOption);
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
