#pragma once

#include <vector>

#include "Parser.h"

namespace summer
{
	struct MatchContent
	{
		int mLowerBound;
		int mUpperBound;
	};

	struct Edge
	{
		MatchContent mMatchContent;
		Status* mStartStatus;
		Status* mEndStatus;
	};

	struct Status
	{
		std::vector<Edge*> mInEdges;
		std::vector<Edge*> mOutEdges;
		bool mIsFinal;
	};

	class FA
	{
	public:
		FA() {};

		FA& And(const FA &);
		FA& Or(const FA &);
		FA& Rep(const FA &);
	};
}