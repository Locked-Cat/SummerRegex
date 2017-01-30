#include <cassert>

#include "../include/DFA.h"

using namespace summer;

ID DFA::AddStatus(bool isFinal)
{
	auto id = static_cast<ID>(mStatus.size());
	mStatus.insert(std::make_pair(id, DFAStatus(isFinal)));
	return id;
}

void summer::DFA::AddEdge(ID src, ID dst, char_t match)
{
	mEdges.push_back(Edge{ src, dst, match });

	mStatus[src].mOutEdges.push_back(mEdges.size() - 1);
	mStatus[dst].mInEdges.push_back(mEdges.size() - 1);
}

ID summer::DFA::Goto(ID current, char_t c)
{
	assert(mStatus.find(current) != mStatus.end());

	for (auto edge : mStatus[current].mOutEdges)
	{
		if (mEdges[edge].mMatch == c)
		{
			return mEdges[edge].mDst;
		}
	}
	return -1;
}

bool summer::DFA::IsFinal(ID id)
{
	return mStatus[id].mIsFinal;
}
