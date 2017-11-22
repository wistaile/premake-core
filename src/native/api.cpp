#include "api.h"

std::map<std::string, ValueDef*> apis;

void addApi(ValueDef *definition)
{
	apis.insert(std::make_pair(definition->name, definition));
}

ValueDef* getApi(const std::string &name)
{
	return apis.at(name);
}
