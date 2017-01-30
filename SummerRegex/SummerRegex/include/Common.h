#pragma once

#include <string>
#include <memory>
#include <vector>

namespace summer
{
	using string_t = std::string;
	using char_t = string_t::value_type;
	using ID = int;

	struct Edge
	{
		ID mSrc, mDst;
		char_t mMatch;
	};

	struct Status
	{
		std::vector<size_t> mInEdges;
		std::vector<size_t> mOutEdges;
	};
}
