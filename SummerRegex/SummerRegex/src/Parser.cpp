#include <cctype>

#include "../include/Parser.h"
#include "../include/Exception.h"

using namespace summer;

FA Parser::Parse(const string_t str)
{
	mStr = str;
	mPos = mStr.begin();
	mFA.reset(new FA);

	R();
}

bool Parser::HasNext() const
{
	return mPos < mStr.cend() - 1;
}

Parser::char_t Parser::PeekNext() const
{
	auto nextPos = mPos + 1;
	if (nextPos < mStr.cend())
	{
		return *nextPos;
	}
	return EOF;
}

void summer::Parser::MoveForward()
{
	mPos += 1;
}

void Parser::R()
{
	E();
}

void Parser::E()
{
	T();
	EQuote();
}

FA Parser::C()
{
	auto c = PeekNext();
	
	if (std::isprint(c))
	{
		MoveForward();
	}
}

int Parser::Number()
{
	auto number = 0;
	auto c = PeekNext();

	if (!std::isdigit(c))
	{
		throw IllegalNumber(mPos - mStr.cbegin());
	}

	number = number * 10 + (c - '0');
	MoveForward();
	while (std::isdigit(c = PeekNext()))
	{
		MoveForward();
		number = number * 10 + (c - '0');
	}
	
	return number;
}