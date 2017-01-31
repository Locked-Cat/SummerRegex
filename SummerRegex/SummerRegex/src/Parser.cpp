#include <cctype>
#include <algorithm>

#include "../include/Parser.h"

using namespace summer;

AST Parser::Parse(const string_t& str)
{
	mTokens = tokenizer.Tokenize(str);
	mPos = mTokens.begin();

	return ParseAlt();
}

AST Parser::ParseAlt()
{
	auto begin = ParseAltBegin();
	auto end = ParseAltEnd();

	return std::unique_ptr<AltNode>(new AltNode(std::move(begin), std::move(end)));
}

AST Parser::ParseAltBegin()
{
	return ParseConcat();
}

AST Parser::ParseAltEnd()
{
	auto token = GetToken();
	if (token.mType == TokenType::TOKEN_META && token.mContent.c == EOF)
	{
		return nullptr;
	}
	else
	{
		if (token.mType != TokenType::TOKEN_META || token.mContent.c != '|')
		{
			throw;
		}
		else
		{
			MoveForward();
			auto end = ParseAlt();
			return std::move(end);
		}
	}
}

AST Parser::ParseConcat()
{
	auto begin = ParseConcatBegin();
	auto end = ParseConcatEnd();
	return std::unique_ptr<ConcatNode>(new ConcatNode(std::move(begin), std::move(end)));
}

AST Parser::ParseConcatBegin()
{
	auto base = ParseBase();
	auto token = GetToken();

	if (token.mType == TokenType::TOKEN_META && token.mContent.c == '*')
	{
		MoveForward();
		return std::unique_ptr<ClosureNode>(new ClosureNode(std::move(base)));
	}
	else
	{
		return std::move(base);
	}
}

AST Parser::ParseConcatEnd()
{
	auto token = GetToken();
	if (token.mType == TokenType::TOKEN_META && token.mContent.c == EOF)
	{
		return nullptr;
	}
	else
	{
		return ParseConcat();
	}
}

AST Parser::ParseBase()
{
	auto token = GetToken();
	if (token.mType == TokenType::TOKEN_META && token.mContent.c == '(')
	{
		MoveForward();
		auto alt = ParseAlt();
		token = GetToken();
		if ( token.mType != TokenType::TOKEN_META || token.mContent.c != ')')
		{
			throw;
		}
		MoveForward();
		return alt;
	}
	else
	{
		std::vector<char_t> charRange;
		if (token.mType == TokenType::TOKEN_META && token.mContent.c == '[')
		{
			MoveForward();
			auto end = std::find_if(mPos, mTokens.end(), [](Token& token) {return token.mType == TokenType::TOKEN_META && token.mContent.c == ']'; });
			if (end == mTokens.end())
			{
				throw;
			}

			auto start = std::find_if(mPos, end, [](Token& token) {return token.mType == TokenType::TOKEN_META && token.mContent.c == '-'; });
			while (start != end)
			{
				if ((start - 1)->mType == TokenType::TOKEN_CHAR && (start + 1)->mType == TokenType::TOKEN_CHAR)
				{
					auto lower = (start - 1)->mContent.c;
					auto upper = (start + 1)->mContent.c;

					if (lower > upper)
					{
						throw;
					}
					else
					{
						for (auto c = lower; c <= upper; ++c)
						{
							charRange.push_back(c);
						}
					}

					(start - 1)->mType = TokenType::TOKEN_PLACEHOLDER;
					start->mType = TokenType::TOKEN_PLACEHOLDER;
					(start + 1)->mType = TokenType::TOKEN_PLACEHOLDER;

					start = std::find_if(start + 2, end, [](Token& token) {return token.mType == TokenType::TOKEN_META && token.mContent.c == '-'; });
				}
				else
				{
					throw;
				}
			}

			while (mPos != end)
			{
				auto token = GetToken();

				if (token.mType == TokenType::TOKEN_ESCAPE)
				{
					InsertEscape(token, charRange);
				}
				else
				{
					if (token.mType == TokenType::TOKEN_CHAR)
					{
						charRange.push_back(token.mContent.c);
					}
					else
					{
						if (token.mType == TokenType::TOKEN_PLACEHOLDER)
						{
							MoveForward();
							continue;
						}
						else
						{
							throw;
						}
					}
				}
				MoveForward();
			}
		}
		else
		{
			if (token.mType == TokenType::TOKEN_ESCAPE)
			{
				InsertEscape(token, charRange);
			}
			else
			{
				if (token.mType == TokenType::TOKEN_CHAR)
				{
					charRange.push_back(token.mContent.c);
				}
				else
				{
					throw;
				}
			}
		}
		std::sort(charRange.begin(), charRange.end());
		auto last = std::unique(charRange.begin(), charRange.end());
		MoveForward();
		return std::unique_ptr<CharRangeNode>(new CharRangeNode(std::vector<char_t>(charRange.begin(), last)));
	}
}

void Parser::InsertEscape(Token & token, std::vector<char_t>& charRange)
{
	switch (token.mContent.escape)
	{
	case EscapeType::ESCAPE_NUM:
	{
		for (auto c = '0'; c <= '9'; ++c)
		{
			charRange.push_back(c);
		}
		break;
	}
	case EscapeType::ESCAPE_SPACE:
	{
		charRange.push_back(' ');
		charRange.push_back('\t');
		charRange.push_back('\n');
		break;
	}
	case EscapeType::ESCAPE_ALPHA:
	{
		for (auto c = 'a'; c <= 'z'; ++c)
		{
			charRange.push_back(c);
			charRange.push_back('A' + c - 'a');
		}
		break;
	}
	default:
		break;
	}
}

Token Parser::GetToken()
{
	if (mPos != mTokens.end())
	{
		return *mPos;
	}
	else
	{
		throw;
	}
}

void Parser::MoveForward()
{
	mPos += 1;
}
