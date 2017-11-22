#include "filter.h"


std::vector<Filter> ctxStack;


void pushFilter(const Filter &ctx)
{
	if (ctxStack.empty())
		ctxStack.push_back(ctx);
	else
		ctxStack.push_back(Filter(ctxStack.back(), ctx));
}

void popFilter()
{
	ctxStack.pop_back();
}

void topFilter(const Filter &ctx)
{

}
