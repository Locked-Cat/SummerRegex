#include <cassert>
#include <list>
#include <set>
#include <algorithm>
#include <cstdio>

#include "../include/DFA.h"

using namespace summer;
using namespace std;

DFAStatus DFA::InsertStatus(bool isFinal)
{
	mStatus.push_back(DFAStatus(new DFAStatusTag()));
	mStatus.back()->isFinal = isFinal;
	return mStatus.back();
}

void DFA::InsertEdge(DFAStatus src, DFAStatus dst, char_t match)
{
	mEdges.push_back(DFAEdge(new DFAEdgeTag()));
	auto edge = mEdges.back();
	edge->src = src;
	edge->dst = dst;
	edge->match = match;

	src->outEdges.push_back(edge);
	dst->inEdges.push_back(edge);
}

DFAStatus DFA::Goto(DFAStatus current, char_t c)
{
	for (auto edge :current->outEdges)
	{
		if (edge->match == c)
		{
			return edge->dst;
		}
	}

	return nullptr;
}

void DFA::Minimize()
{
	list<set<DFAStatus>> minimizationSet{ 2, set<DFAStatus>() };
	auto finalSet = minimizationSet.begin();
	auto nonFinalSet = finalSet++;

	for (auto s : mStatus)
	{
		if (s->isFinal)
		{
			finalSet->insert(s);
		}
		else
		{
			nonFinalSet->insert(s);
		}
	}

	if (nonFinalSet->empty())
	{
		return;
	}

	for (auto iter = minimizationSet.begin(); iter != minimizationSet.end();)
	{
		if (iter->size() == 1)
		{
			iter++;
		}
		else
		{
			auto hasSplit = false;
			for (auto s : *iter)
			{
				for (auto& edge : s->outEdges)
				{
					if (iter->find(edge->dst) == iter->end())
					{
						iter->erase(s);
						minimizationSet.push_front(set<DFAStatus>{ s });
						hasSplit = true;
						break;
					}
				}

				if (hasSplit)
				{
					break;
				}
			}
			if (!hasSplit)
			{
				iter++;
			}
		}
	}

	vector<DFAStatus> oldStatus;
	oldStatus.swap(mStatus);
	vector<DFAEdge> oldEdges;
	oldEdges.swap(mEdges);
	map<DFAStatus, DFAStatus> oldToNew;
	
	auto flag = true;
	for (auto& cluster : minimizationSet)
	{
		auto newStatus = InsertStatus((*(cluster.begin()))->isFinal);
		if (flag && cluster.find(start) != cluster.end())
		{
			start = newStatus;
			flag = false;
		}

		for (auto s : cluster)
		{
			oldToNew[s] = newStatus;
		}
	}

	for (auto& cluster : minimizationSet)
	{
		for (auto s : cluster)
		{
			for (auto edge : s->outEdges)
			{
				InsertEdge(oldToNew[edge->src], oldToNew[edge->dst], edge->match);
			}
		}
	}
}

void DFA::Print()
{
	printf("Start: %d\n", Index(start));
	for (auto edge : mEdges)
	{
		printf("%d-->%d: %c\n", Index(edge->src), Index(edge->dst), edge->match);
	}
}

int DFA::Index(DFAStatus status)
{
	return static_cast<int>(distance(mStatus.begin(), find(mStatus.begin(), mStatus.end(), status)));
}