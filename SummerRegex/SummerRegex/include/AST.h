#pragma once

#include <memory>
#include <utility>
#include <set>

#include "Types.h"
#include "NFA.h"

namespace summer
{
	class ASTNode
	{
	public:
		virtual ~ASTNode() = 0 {};
		virtual NFA Construct() = 0;
	};

	class CharSetNode
		: public ASTNode
	{
	public:
		CharSetNode(const std::set<char_t>& charSet, bool exclude)
			: mCharSet(charSet), mExclude(exclude) {}

		NFA Construct() override;
	private:
		std::set<char_t> mCharSet;
		bool mExclude;
	};

	class AltNode
		: public ASTNode
	{
	public:
		AltNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
			: mLeft(std::move(left)), mRight(std::move(right)) {}

		NFA Construct() override;
	private:
		std::unique_ptr<ASTNode> mLeft, mRight;
	};

	class ConcatNode
		: public ASTNode
	{
	public:
		ConcatNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
			: mLeft(std::move(left)), mRight(std::move(right)) {}

		NFA Construct() override;
	private:
		std::unique_ptr<ASTNode> mLeft, mRight;
	};

	class LoopNode
		: public ASTNode
	{
	public:
		LoopNode(std::unique_ptr<ASTNode> closure, int min, int max)
			: mBody(std::move(closure)), mMin(min), mMax(max) {}

		NFA Construct() override;
	private:
		NFA Aux(NFA& body);
		std::unique_ptr<ASTNode> mBody;
		int mMin, mMax;
	};
}
