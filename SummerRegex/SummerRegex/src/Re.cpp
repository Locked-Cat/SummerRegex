#include "../include/Re.h"
#include "../include/NFA.h"

using namespace summer;

Re::Re(const string_t& pattern)
{
	auto ast = mParser.Parse(pattern);
	NFA m;
	ast->Construct(m);
	m.ConvertToDFA(mAutomat);
}

bool Re::Match(const string_t& str)
{
	auto status = mAutomat.Start();

	for (auto c : str)
	{
		status = mAutomat.Goto(status, c);
		if (status == -1)
			return false;
	}

	return mAutomat.IsFinal(status);
}

SearchResult Re::Search(const string_t & str)
{
	mAutomat.Print();
	auto result = SearchResult();
	auto pos = str.begin();

	while (pos != str.end())
	{
		auto status = mAutomat.Start();
		auto lastStatus = -1;
		auto end = pos;

		while (end != str.end() && status != -1)
		{
			lastStatus = status;
			status = mAutomat.Goto(status, *end);
			end++;
		}

		if (status != -1)
		{
			if (mAutomat.IsFinal(status))
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
			if (mAutomat.IsFinal(lastStatus))
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

