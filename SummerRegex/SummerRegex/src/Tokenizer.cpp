#include "..\include\Tokenizer.h"

using namespace summer;

std::vector<Token> Tokenizer::Tokenize(const string_t & str)
{
	std::vector<Token> result;
	for (auto i = 0; i < str.length();)
	{
		if (str[i] == '\\')
		{
			switch (str[i + 1])
			{
			case 'd':
				result.push_back(Token{ TokenType::TOKEN_ESCAPE, EscapeType::ESCAPE_NUM });
				break;
			case 'w':
				result.push_back(Token{ TokenType::TOKEN_ESCAPE, EscapeType::ESCAPE_ALPHA });
				break;
			case 's':
				result.push_back(Token{ TokenType::TOKEN_ESCAPE, EscapeType::ESCAPE_SPACE });
				break;
			case '\\':
			case '*':
			case '[':
			case ']':
			case '-':
			case '(':
			case ')':
			case '|':
				result.push_back(Token{ TokenType::TOKEN_CHAR, str[i + 1] });
				break;
			default:
				throw;
			}
			i += 2;
		}
		else
		{
			switch (str[i])
			{
			case '\\':
			case '*':
			case '[':
			case ']':
			case '-':
			case '(':
			case ')':
			case '|':
				result.push_back(Token{ TokenType::TOKEN_META, str[i] });
				break;
			default:
				result.push_back(Token{ TokenType::TOKEN_CHAR, str[i] });
				break;
			}
			i += 1;
		}
	}

	result.push_back(Token{ TokenType::TOKEN_META, EOF });
	return result;
}
