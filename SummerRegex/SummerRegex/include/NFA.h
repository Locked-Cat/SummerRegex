#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>

#include "Types.h"
#include "DFA.h"

namespace summer
{
	struct NFAStatusTag;
	using NFAStatus = std::shared_ptr<NFAStatusTag>;
	struct NFAEdgeTag;
	using NFAEdge = std::shared_ptr<NFAEdgeTag>;

	struct NFAEdgeTag
	{
		NFAStatus src, dst;
		char_t match;
	};

	struct NFAStatusTag
	{
		std::vector<NFAEdge> inEdges;
		std::vector<NFAEdge> outEdges;
	};

	class NFA
	{
		friend NFA Merge(NFA& a, NFA& b);
	public:
		static const int sEpsilon = -1;

		NFA(bool init = true) 
		{
			if (init)
			{
				start = InsertStatus();
				end = InsertStatus();
			}
		}

		NFA(const NFA&) = default;
		NFA& operator=(const NFA&) = default;
		NFA(NFA&&) = default;
		NFA& operator=(NFA&&) = default;

		NFAStatus InsertStatus();
		void InsertEdge(NFAStatus src, NFAStatus dst, char_t match);
		DFA ConvertToDFA();
		NFA Clone();
		void Print();

		NFAStatus start;
		NFAStatus end;
	private:
		int Index(NFAStatus status);
		std::set<NFAStatus> EpsilonExtend(const std::set<NFAStatus>& ids);
		 
		std::vector<NFAStatus> mStatus;
		std::vector<NFAEdge> mEdges;
		std::set<char_t> mCharSet;
	};
}
