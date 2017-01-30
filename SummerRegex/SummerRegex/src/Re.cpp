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

