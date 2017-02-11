#include "../include/Re.h"
#include "../include/NFA.h"

using namespace summer;

Re::Re(const string_t& pattern)
{
	auto ast = mParser.Parse(pattern);
	auto nfa = ast->Construct();
	mAutomat = nfa.ConvertToDFA();
}

bool Re::Match(const string_t& str)
{
	auto status = mAutomat.start;

	for (auto c : str)
	{
		status = mAutomat.Goto(status, c);
		if (status == nullptr)
		{
			return false;
		}
	}

	return status->isFinal;
}

SearchResult Re::Search(const string_t & str)
{
	auto result = SearchResult();
	auto pos = str.begin();

	while (pos != str.end())
	{
		auto status = mAutomat.start;
		auto lastStatus = status;
		auto end = pos;

		while (end != str.end() && status != nullptr)
		{
			lastStatus = status;
			status = mAutomat.Goto(status, *end);
			end++;
		}

		if (status != nullptr)
		{
			if (status->isFinal)
			{
				result.location = std::distance(str.begin(), pos);
				result.length = std::distance(pos, end);
				result.success = true;
				result.value = str.substr(result.location, result.length);
				return result;
			}
		}
		else
		{
			if (lastStatus->isFinal)
			{
				result.location = std::distance(str.begin(), pos);
				result.length = std::distance(pos, end - 1);
				result.success = true;
				result.value = str.substr(result.location, result.length);
				return result;
			}
		}
		pos++;
	}
	return result;
}

