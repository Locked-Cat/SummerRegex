#pragma once

#include "Common.h"
#include "DFA.h"
#include "Parser.h"
#include "Common.h"

namespace summer
{
	struct SearchResult
	{
		bool success;
		string_t value;
		size_t location;
		size_t length;

		SearchResult()
			: success(false), value(""), location(-1), length(-1) {}
	};

	class Re
	{
	public:
		Re(const string_t& pattern);
		bool Match(const string_t& str);
		SearchResult Search(const string_t& str);
	private:
		DFA mAutomat;
		Parser mParser;
	};
}
