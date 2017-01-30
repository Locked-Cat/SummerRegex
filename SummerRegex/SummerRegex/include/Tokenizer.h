#pragma once

#include <vector>

#include "Common.h"

namespace summer
{
	enum TokenType
	{
		TOKEN_CHAR,
		TOKEN_META,
		TOKEN_ESCAPE,
		TOKEN_PLACEHOLDER
	};

	enum EscapeType
	{
		ESCAPE_NUM,
		ESCAPE_ALPHA,
		ESCAPE_SPACE
	};

	struct Token
	{
		TokenType mType;

		Token(TokenType type, char_t c)
			: mType(type)
		{
			mContent.c = c;
		}

		Token(TokenType type, EscapeType e)
			:mType(type)
		{
			mContent.escape = e;
		}

		union
		{
			char_t c;
			EscapeType escape;
		}mContent;
	};

	class Tokenizer
	{
	public:
		Tokenizer() {}
		std::vector<Token> Tokenize(const string_t& str);
	};
}