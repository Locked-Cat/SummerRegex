#pragma once

#include "Common.h"
#include "DFA.h"
#include "Parser.h"

namespace summer
{
	class Re
	{
	public:
		Re(const string_t& pattern);
		bool Match(const string_t& str);
	private:
		DFA mAutomat;
		Parser mParser;
	};
}
