#pragma once

#include <vector>
#include <string>
#include <map>

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

void addApi(ValueDef *definition);
ValueDef* getApi(const std::string &name);
