#include <cassert>
#include <list>
#include <set>
#include <algorithm>
#include <cstdio>

#include "../include/DFA.h"

using namespace summer;
using namespace std;

ID DFA::AddStatus(bool isFinal)
{
	auto id = static_cast<ID>(mStatus.size());
	mStatus.insert(std::make_pair(id, DFAStatus(isFinal)));
	mIDs.insert(id);
	return id;
}

void summer::DFA::AddEdge(ID src, ID dst, char_t match)
{
	auto edge = Edge{ src, dst, match };
	if (find(mEdges.begin(), mEdges.end(), edge) != mEdges.end())
	{
		return;
	}
	mEdges.push_back(edge);

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

bool DFA::IsFinal(ID id)
{
	return mStatus[id].mIsFinal;
}

void DFA::Minimize()
{
	list<set<ID>> minimizationSet{ 2, set<ID>() };
	auto finalSet = minimizationSet.begin();
	auto nonFinalSet = finalSet++;

	for (auto id : mIDs)
	{
		if (IsFinal(id))
		{
			finalSet->insert(id);
		}
		else
		{
			nonFinalSet->insert(id);
		}
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
			for (auto id : *iter)
			{
				for (auto& edge : mStatus[id].mOutEdges)
				{
					if (iter->find(mEdges[edge].mDst) == iter->end())
					{
						iter->erase(id);
						minimizationSet.push_front(set<ID>{ id });
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

	map<ID, DFAStatus> oldStatus;
	oldStatus.swap(mStatus);
	vector<Edge> oldEdges;
	oldEdges.swap(mEdges);
	set<ID> oldIDs;
	oldIDs.swap(mIDs);
	map<ID, ID> oldToNew;

	auto flag = true;
	for (auto& cluster : minimizationSet)
	{
		auto newID = AddStatus(oldStatus[*cluster.begin()].mIsFinal);
		if (flag && cluster.find(mStart) != cluster.end())
		{
			mStart = newID;
			flag = false;
		}

		for (auto id : cluster)
		{
			oldToNew.insert(make_pair(id, newID));
		}
	}

	for (auto& cluster : minimizationSet)
	{
		for (auto id : cluster)
		{
			for (auto edge : oldStatus[id].mOutEdges)
			{
				AddEdge(oldToNew[oldEdges[edge].mSrc], oldToNew[oldEdges[edge].mDst], oldEdges[edge].mMatch);
			}
		}
	}
}

void DFA::Print()
{
	printf("Start: %d\n", mStart);
	for (auto& edge : mEdges)
	{
		printf("%d--%c-->%d\n", edge.mSrc, edge.mMatch, edge.mDst);
	}
}
