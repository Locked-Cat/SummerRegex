#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>

#include "Common.h"
#include "DFA.h"

namespace summer
{
	class NFA
	{
	public:
		static const int sEpsilon = -1;

		NFA()
			: mStart(-1), mEnd(-1) {}

		ID& Start()
		{
			return mStart;
		}

		ID& End()
		{
			return mEnd;
		}

		ID AddStatus();
		void AddEdge(ID src, ID dst, char_t match);
		void ConvertToDFA(DFA& m);
		void Print();
	private:
		std::set<ID> EpsilonExtend(const std::set<ID>& ids);

		ID mStart, mEnd;
		std::map<ID, Status> mStatus;
		std::vector<Edge> mEdges;
		std::set<char_t> mCharSet;
	};
}
