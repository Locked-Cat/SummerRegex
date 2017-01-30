#pragma once

#include <vector>
#include <map>

#include "Common.h"

namespace summer
{
	struct DFAStatus
		: public Status
	{
		DFAStatus(bool isFinal = false)
			: Status(), mIsFinal(isFinal) {}
		bool mIsFinal;
	};

	class DFA
	{
	public:
		ID AddStatus(bool isFinal = false);
		void AddEdge(ID src, ID dst, char_t match);
		
		ID& Start()
		{
			return mStart;
		}

		ID Goto(ID current, char_t c);
		bool IsFinal(ID id);
	private:
		ID mStart;
		std::map<ID, DFAStatus> mStatus;
		std::vector<Edge> mEdges;
	};
}
