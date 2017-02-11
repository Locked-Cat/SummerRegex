#include <cassert>
#include <cstdio>
#include <queue>
#include <algorithm>
#include <list>
#include <map>

#include "../include/NFA.h"

using namespace summer;
using namespace std;

NFAStatus NFA::InsertStatus()
{
	mStatus.push_back(NFAStatus(new NFAStatusTag()));
	return mStatus.back();
}

void NFA::InsertEdge(NFAStatus src, NFAStatus dst, char_t match)
{
	mEdges.push_back(NFAEdge(new NFAEdgeTag()));
	auto edge = mEdges.back();
	edge->src = src;
	edge->dst = dst;
	edge->match = match;

	if (match != sEpsilon)
	{
		mCharSet.insert(match);
	}

	src->outEdges.push_back(edge);
	dst->inEdges.push_back(edge);
}

int NFA::Index(NFAStatus status)
{
	return static_cast<int>(distance(mStatus.begin(), find(mStatus.begin(), mStatus.end(), status)));
}

set<NFAStatus> NFA::EpsilonExtend(const set<NFAStatus>& statusSet)
{
	set<NFAStatus> result;

	for (auto s : statusSet)
	{
		queue<NFAStatus> q;
		q.push(s);

		while (!q.empty())
		{
			auto status = q.front();
			q.pop();
			result.insert(status);

			for (auto edge : status->outEdges)
			{
				if (edge->match == sEpsilon)
				{
					q.push(edge->dst);
				}
			}
		}
	}

	return result;
}

DFA NFA::ConvertToDFA()
{
	DFA m(false);
	vector<set<NFAStatus>> statusCluster;
	vector<DFAStatus> DFAStatusSet;

	auto init = EpsilonExtend({ start });
	statusCluster.push_back(init);
	auto isFinal = (init.find(end) != init.end());
	m.start = m.InsertStatus(isFinal);
	DFAStatusSet.push_back(m.start);
	
	queue<size_t> q;
	q.push(0);
	while (!q.empty())
	{
		auto index = q.front();
		q.pop();

		for (auto c : mCharSet)
		{
			set<NFAStatus> NFAStatusSet;
			for (auto s: statusCluster[index])
			{
				for (auto edge : s->outEdges)
				{
					if (edge->match == c)
					{
						NFAStatusSet.insert(edge->dst);
					}
				}
			}
			
			if (NFAStatusSet.size() == 0)
				continue;

			NFAStatusSet = EpsilonExtend(NFAStatusSet);

			auto iter = find(statusCluster.begin(), statusCluster.end(), NFAStatusSet);
			if (iter == statusCluster.end())
			{
				statusCluster.push_back(NFAStatusSet);
				isFinal = (NFAStatusSet.find(end) != NFAStatusSet.end());
				DFAStatusSet.push_back(m.InsertStatus(isFinal));
				q.push(statusCluster.size() - 1);
				iter = statusCluster.end() - 1;
			}

			auto dist = distance(statusCluster.begin(), iter);
			m.InsertEdge(DFAStatusSet[index], DFAStatusSet[dist], c);
		}
	}
	m.Minimize();
	return m;
}

NFA NFA::Clone()
{
	NFA m(false);

	m.mCharSet = mCharSet;
	map<NFAStatus, NFAStatus> oldToNew;
	for (auto s : mStatus)
	{
		m.mStatus.push_back(NFAStatus(new NFAStatusTag()));
		oldToNew[s] = m.mStatus.back();
	}

	for (auto edge : mEdges)
	{
		m.InsertEdge(oldToNew[edge->src], oldToNew[edge->dst], edge->match);
	}

	m.start = oldToNew[start];
	m.end = oldToNew[end];

	return m;
}

void NFA::Print()
{
	printf("Start: %d End: %d\n", Index(start), Index(end));
	for (auto edge : mEdges)
	{
		if (edge->match != sEpsilon)
		{
			printf("%d-->%d: %c\n", Index(edge->src), Index(edge->dst), edge->match);
		}
		else
		{
			printf("%d-->%d: %s\n", Index(edge->src), Index(edge->dst), "epsilon");
		}
	}
}

NFA summer::Merge(NFA& a, NFA& b)
{
	NFA m;
	
	m.mCharSet.swap(a.mCharSet);
	m.mCharSet.insert(b.mCharSet.begin(), b.mCharSet.end());
	b.mCharSet.clear();

	m.mEdges.swap(a.mEdges);
	m.mEdges.insert(m.mEdges.end(), b.mEdges.begin(), b.mEdges.end());
	b.mEdges.clear();

	m.mStatus.insert(m.mStatus.end(), a.mStatus.begin(), a.mStatus.end());
	m.mStatus.insert(m.mStatus.end(), b.mStatus.begin(), b.mStatus.end());
	b.mStatus.clear();

	a.start.reset();
	a.end.reset();
	b.start.reset();
	b.end.reset();

	return m;
}
