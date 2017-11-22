#pragma once

#include <vector>
#include <string>
#include <map>
#include <assert.h>

#include "api.h"
#include "filter.h"


class Value
{
public:
	Value(ValueDef *def, bool val) : def(def), val(val) { assert(def->type == VT_Bool); }
	Value(ValueDef *def, std::string val) : def(def), val(std::move(val)) { assert(def->type == VT_String || def->type == VT_Path); }
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

class Block
{
public:
	Filter filter;

	std::map<std::string, Value> values;

	std::vector<Block*> children;
};


void initContext();
