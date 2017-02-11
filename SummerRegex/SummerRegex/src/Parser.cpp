#include <cctype>
#include <algorithm>
#include <exception>
#include <cctype>

#include "../include/Parser.h"

using namespace summer;
using namespace std;

set<char_t> Parser::sLegalCharSet({
	' ','!','"','#','$','%','&','\'',
	'(',')','*','+',',','-','.','/',
	'0','1','2','3','4','5','6','7',
	'8','9',':',';','<','=','>','?',
	'@','A','B','C','D','E','F','G',
	'H','I','J','K','L','M','N','O',
	'P','Q','R','S','T','U','V','W',
	'X','Y','Z','[','\\',']','^','_',
	'`','a','b','c','d','e','f','g',
	'h','i','j','k','l','m','n','o',
	'p','q','r','s','t','u','v','w',
	'x','y','z','{','|','}','~','\n',
	'\t','\r' });

AST Parser::Parse(const string_t& str)
{
	mTokens = tokenizer.Tokenize(str);
	mPos = mTokens.begin();
	return ParseAlt();
}

AST Parser::ParseAlt()
{
	auto concat = ParseConcat();

	auto token =  GetToken();
	if (token->GetType() == META)
	{
		auto m = GetValue<MetaToken>(*token);
		if (m == '|')
		{
			mPos++;
			auto alt = ParseAlt();
			return unique_ptr<AltNode>(new AltNode(move(concat), move(alt)));
		}

		if (m == ')')
		{
			return concat;
		}

		throw;
	}

	if (token->GetType() == END)
	{
		return concat;
	}

	throw;
}

AST Parser::ParseConcat()
{
	auto base = ParseBase();

	auto token = GetToken();
	if (token->GetType() == META)
	{
		auto m = GetValue<MetaToken>(*token);
		if (m == '[' || m == '\\' || m == '(')
		{
			auto concat = ParseConcat();
			return unique_ptr<ConcatNode>(new ConcatNode(move(base), move(concat)));
		}

		if (m == ')' || m == '|')
		{
			return base;
		}

		throw;
	}

	if (token->GetType() == CHAR)
	{
		auto concat = ParseConcat();
		return unique_ptr<ConcatNode>(new ConcatNode(move(base), move(concat)));
	}

	if (token->GetType() == END)
	{
		return base;
	}

	throw;
}

AST Parser::ParseBase()
{
	auto factor = ParseFactor();

	auto token =  GetToken();
	if (token->GetType() == META)
	{
		auto m = GetValue<MetaToken>(*token);
		if (m == '{' || m == '*' || m == '+' || m == '?')
		{
			auto loop = ParseLoop();
			return unique_ptr<LoopNode>(new LoopNode(std::move(factor), loop.first, loop.second));
		}

		if (m == '[' || m == '\\' || m == '(' || m == '|' || m == ')')
		{
			return factor;
		}
	}

	if (token->GetType() == END || token->GetType() == CHAR)
	{
		return factor;
	}

	throw;
}

AST Parser::ParseFactor()
{
	auto token =  GetToken();
	
	if (token->GetType() == META)
	{
		switch (GetValue<MetaToken>(*token))
		{
		case '[':
			return ParseCharSet();
		case '\\':
		{
			mPos++;
			token = GetToken();
			if (token->GetType() == META)
			{
				mPos--;
				auto c = ParseChar();
				return unique_ptr<CharSetNode>(new CharSetNode(set<char_t>{c}, false));
			}
			
			if(token->GetType() == CHAR)
			{
				mPos--;
				return ParseCharSet();
			}
			
			throw;
		}
		case '(':
		{
			mPos++;
			auto alt = ParseAlt();
			token =  GetToken();
			if (token->GetType() == META && GetValue<MetaToken>(*token) == ')')
			{
				mPos++;
				return alt;
			}

			throw;
		}
		default:
			throw;
		}
	}

	if(token->GetType() == CHAR)
	{
		auto c = ParseChar();
		return unique_ptr<CharSetNode>(new CharSetNode(set<char_t>{c}, false));
	}

	throw;
}

pair<int, int> Parser::ParseLoop()
{
	auto token =  GetToken();
	if (token->GetType() == META)
	{
		switch (GetValue<MetaToken>(*token))
		{
		case '{':
		{
			mPos++;
			int min, max;
			token = GetToken();
			if (token->GetType() == META && GetValue<MetaToken>(*token) == ',')
			{
				mPos++;
				min = 0;
				max = ParseNumber();
			}
			else
			{
				min = max = ParseNumber();
				
				token = GetToken();
				if (token->GetType() == META && GetValue<MetaToken>(*token) == ',')
				{
					mPos++;
					token = GetToken();

					if (token->GetType() == META && GetValue<MetaToken>(*token) == '}')
					{
						mPos++;
						return make_pair(min, -1);
					}
					else
					{
						max = ParseNumber();
					}
				}
			}

			token =  GetToken();
			if (token->GetType() == META && GetValue<MetaToken>(*token) == '}')
			{
					mPos++;
					return make_pair(min, max);
			}

			throw;
		}
		case '*':
			mPos++;
			return make_pair(0, -1);
		case '+':
			mPos++;
			return make_pair(1, -1);
		case '?':
			mPos++;
			return make_pair(0, 1);
		default:
			throw;
		}
	}

	throw;
}

AST Parser::ParseCharSet()
{
	auto token =  GetToken();
	if (token->GetType() == META)
	{
		if (GetValue<MetaToken>(*token) == '[')
		{
			mPos++;
			auto exclude = false;
			token = GetToken();

			if (token->GetType() == META && GetValue<MetaToken>(*token) == '^')
			{
				mPos++;
				exclude = true;
			}

			set<char_t> charSet;
			ParseCharRange(charSet);
			auto& token =  GetToken();
			if (token->GetType() == META && GetValue<MetaToken>(*token) == ']')
			{
				mPos++;
				return unique_ptr<CharSetNode>(new CharSetNode(charSet, exclude));
			}

			throw;
		}

		if (GetValue<MetaToken>(*token) == '\\')
		{
			mPos++;
			set<char_t> charSet;
			ParseX(charSet);
			return unique_ptr<CharSetNode>(new CharSetNode(charSet, false));
		}

		if (GetValue<MetaToken>(*token) == '.')
		{
			mPos++;
			return unique_ptr<CharSetNode>(new CharSetNode(sLegalCharSet, false));
		}

		throw;
	}

	throw;
}

void Parser::ParseCharRange(set<char_t>& charSet)
{
	ParseCharBase(charSet);
	
	auto token =  GetToken();
	if (token->GetType() == CHAR || (token->GetType() == META && GetValue<MetaToken>(*token) == '\\'))
	{
		ParseCharRange(charSet);
	}
}

void Parser::ParseCharBase(set<char_t>& charSet)
{
	auto c0 = ParseChar();

	auto token =  GetToken();
	if (token->GetType() == META && GetValue<MetaToken>(*token) == '-')
	{
		mPos++;
		auto c1 = ParseChar();
		if (c0 <= c1)
		{
			for (auto c = c0; c <= c1; ++c)
			{
				charSet.insert(c);
			}
			return;
		}
		
		throw;
	}
	else
	{
		charSet.insert(c0);
	}
}

void Parser::ParseX(set<char_t>& charSet)
{
	auto token =  GetToken();
	if (token->GetType() == CHAR)
	{
		switch (GetValue<CharToken>(*token))
		{
		case 'w':
		{
			for (auto c = 'a'; c <= 'z'; ++c)
			{
				charSet.insert(c);
				charSet.insert(c - 'a' + 'A');
			}
			mPos++;
			return;
		}
		case 'd':
		{
			for (auto c = '0'; c <= '9'; ++c)
			{
				charSet.insert(c);
			}
			mPos++;
			return;
		}
		case 's':
		{
			charSet.insert(' ');
			charSet.insert('\n');
			charSet.insert('\t');
			charSet.insert('\r');
		}
		default:
			throw;
		}
	}

	throw;
}

char_t Parser::ParseChar()
{
	auto token =  GetToken();
	if (token->GetType() == META && GetValue<MetaToken>(*token) == '\\')
	{
		mPos++;
		auto token =  GetToken();
		if (token->GetType() == META)
		{
			auto c =  GetValue<MetaToken>(*token);
			mPos++;
			return c;
		}

		if (token->GetType() == CHAR)
		{
			auto c = GetValue<CharToken>(*token);
			switch (c)
			{
			case 'n':
				c = '\n';
				break;
			case 't':
				c = '\t';
				break;
			case 'r':
				c = '\r';
				break;
			default:
				throw;
			}
			mPos++;
			return c;
		}

		throw;
	}

	if (token->GetType() == CHAR)
	{
		mPos++;
		return GetValue<CharToken>(*token);
	}

	throw;
}

int Parser::ParseNumber()
{
	auto token = GetToken();
	if (token->GetType() == CHAR && isdigit(GetValue<CharToken>(*token)))
	{
		auto value = GetValue<CharToken>(*token) - '0';
		++mPos;
		while (token = GetToken(), token->GetType() == CHAR && isdigit(GetValue<CharToken>(*token)))
		{
			auto value = GetValue<CharToken>(*token) - '0';
			++mPos;
		}

		return value;
	}
	
	throw;
}

shared_ptr<Token> Parser::GetToken()
{
	if (mPos != mTokens.end())
	{
		return *mPos;
	}

	throw;
}
