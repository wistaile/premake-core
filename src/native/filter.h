#pragma once

#include <string>
#include <vector>


class FilterTerm
{
public:
	FilterTerm(const FilterTerm &v) : api(v.api), value(v.value) {}
	FilterTerm(FilterTerm &&v) : api(std::move(v.api)), value(std::move(v.value)) {}

	FilterTerm(const std::string &api, const std::string &value) : api(api), value(value) {}
	FilterTerm(const std::string &api, std::string &&value) : api(api), value(std::move(value)) {}
	FilterTerm(std::string &&api, const std::string &value) : api(std::move(api)), value(value) {}
	FilterTerm(std::string &&api, std::string &&value) : api(std::move(api)), value(std::move(value)) {}

	std::string api;
	std::string value;
};

class FilterOr
{
public:
	FilterOr(const FilterOr &v) : terms(v.terms) {}
	FilterOr(FilterOr &&v) : terms(std::move(v.terms)) {}

	FilterOr(const FilterTerm &v) { terms.push_back(v); }
	FilterOr(FilterTerm &&v) { terms.push_back(std::move(v)); }
	FilterOr(const std::vector<FilterTerm> &v) : terms(v) {}
	FilterOr(std::vector<FilterTerm> &&v) : terms(std::move(v)) {}

	std::vector<FilterTerm> terms; // 'or'; any may match
};

class Filter
{
public:
	Filter(const Filter &v) : predicate(v.predicate) {}
	Filter(Filter &&v) : predicate(std::move(v.predicate)) {}

	Filter(const FilterTerm &v) { predicate.push_back(v); }
	Filter(FilterTerm &&v) { predicate.push_back(std::move(v)); }
	Filter(const std::vector<FilterTerm> &v) { for (auto &&t : v) predicate.push_back(t); }
	Filter(std::vector<FilterTerm> &&v) { for (auto &&t : v) predicate.push_back(std::move(t)); }

	Filter(const FilterOr &v) { predicate.push_back(v); }
	Filter(FilterOr &&v) { predicate.push_back(std::move(v)); }
	Filter(const std::vector<FilterOr> &v) : predicate(v) {}
	Filter(std::vector<FilterOr> &&v) : predicate(std::move(v)) {}

	Filter(const Filter &v, const Filter &v2) : predicate(v.predicate) { append(v2); }

	void append(const Filter &ctx)
	{
		// TODO: this will allow duplicate terms in contexts
		//       should probably remove dupes while merging... probably harmless for now

		for (auto &&t : ctx.predicate)
			predicate.push_back(t);
	}

	std::vector<FilterOr> predicate; // conjunction; all much match
};
