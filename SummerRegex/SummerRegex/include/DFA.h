#pragma once

#include <vector>
#include <map>
#include <set>
#include <memory>

#include "Types.h"

namespace summer
{
	struct DFAStatusTag;
	using DFAStatus = std::shared_ptr<DFAStatusTag>;
	struct DFAEdgeTag;
	using DFAEdge = std::shared_ptr<DFAEdgeTag>;

	struct DFAEdgeTag
	{
		DFAStatus src, dst;
		char_t match;
	};

	struct DFAStatusTag
	{
		std::vector<DFAEdge> inEdges;
		std::vector<DFAEdge> outEdges;
		bool isFinal;
	};

	class DFA
	{
	public:
		DFA(bool init = true) 
		{
			if (init)
			{
				start = InsertStatus();
			}
		}

		DFA(const DFA&) = default;
		DFA& operator=(const DFA&) = default;
		DFA(DFA&&) = default;
		DFA& operator=(DFA&&) = default;

		DFAStatus InsertStatus(bool isFinal = false);
		void InsertEdge(DFAStatus src, DFAStatus dst, char_t match);

		DFAStatus Goto(DFAStatus current, char_t c);
		void Minimize();
		void Print();

		DFAStatus start;
	private:
		int Index(DFAStatus status);

		std::vector<DFAStatus> mStatus;
		std::vector<DFAEdge> mEdges;
	};
}
