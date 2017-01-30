#include <cassert>
#include <cstdio>
#include <queue>
#include <algorithm>

#include "../include/NFA.h"

using namespace summer;

ID NFA::AddStatus()
{
	auto id = static_cast<ID>(mStatus.size());
	mStatus.insert(std::make_pair(id, Status()));
	return id;
}

void NFA::AddEdge(ID src, ID dst, char_t match)
{
	assert(mStatus.find(src) != mStatus.end() && mStatus.find(dst) != mStatus.end());

	mEdges.push_back(Edge{src, dst, match});

	if (match != sEpsilon)
	{
		mCharSet.insert(match);
	}

	mStatus[src].mOutEdges.push_back(mEdges.size() - 1);
	mStatus[dst].mInEdges.push_back(mEdges.size() - 1);
}

void NFA::Print()
{
	printf("Start: %d End: %d\n", mStart, mEnd);
	for (auto& edge : mEdges)
	{
		if (edge.mMatch == sEpsilon)
		{
			printf("%d--%s-->%d\n", edge.mSrc, "null", edge.mDst);
		}
		else
		{
			printf("%d--%c-->%d\n", edge.mSrc, edge.mMatch, edge.mDst);
		}
	}
}

std::set<ID> summer::NFA::EpsilonExtend(const std::set<ID>& ids)
{
	std::set<ID> result;

	for (auto s : ids)
	{
		std::queue<ID> q;
		q.push(s);

		while (!q.empty())
		{
			auto id = q.front();
			q.pop();
			result.insert(id);

			for (auto edge : mStatus[id].mOutEdges)
			{
				if (mEdges[edge].mMatch == sEpsilon)
				{
					q.push(mEdges[edge].mDst);
				}
			}
		}
	}

	return result;
}

void NFA::ConvertToDFA(DFA& m)
{
	std::vector<std::set<ID>> DFAStatus;
	std::vector<ID> DFAIDs;

	auto init = EpsilonExtend({ 0 });
	DFAStatus.push_back(init);
	DFAIDs.push_back(m.AddStatus(init.find(mEnd) != init.end()));
	m.Start() = DFAIDs[0];
	
	std::queue<size_t> q;
	q.push(0);
	while (!q.empty())
	{
		auto index = q.front();
		q.pop();

		for (auto c : mCharSet)
		{
			std::set<ID> status;
			for (auto s : DFAStatus[index])
			{
				for (auto edge : mStatus[s].mOutEdges)
				{
					if (mEdges[edge].mMatch == c)
					{
						status.insert(mEdges[edge].mDst);
					}
				}
			}
			
			if (status.size() == 0)
				continue;

			status = EpsilonExtend(status);

			auto iter = std::find(DFAStatus.begin(), DFAStatus.end(), status);
			if (iter == DFAStatus.end())
			{
				DFAStatus.push_back(status);
				bool isFinal = (status.find(mEnd) != status.end());
				DFAIDs.push_back(m.AddStatus(isFinal));
				q.push(DFAIDs.size() - 1);
				iter = DFAStatus.end() - 1;
			}

			auto dist = std::distance(DFAStatus.begin(), iter);
			m.AddEdge(DFAIDs[index], DFAIDs[dist], c);
		}
	}
}
