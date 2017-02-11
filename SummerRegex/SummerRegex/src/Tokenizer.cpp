#include "..\include\Tokenizer.h"


using namespace summer;
using namespace std;

std::vector<shared_ptr<Token>> Tokenizer::Tokenize(const string_t & str)
{
	vector<shared_ptr<Token>> result;

	for (auto pos = str.begin(); pos != str.end();)
	{
		switch (*pos)
		{
		case '+':
		case '-':
		case '*':
		case '\\':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '?':
		case '|':
		case ',':
		case '^':
		case '.':
			result.push_back(make_shared<MetaToken>(*pos));
			break;
		default:
			result.push_back(make_shared<CharToken>(*pos));
			break;
		}
		pos++;
	}
	result.push_back(make_shared<EndToken>());
	return result;
}
