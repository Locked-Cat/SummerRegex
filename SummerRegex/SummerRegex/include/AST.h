#pragma once

#include <vector>
#include <memory>

#include "Common.h"
#include "NFA.h"

namespace summer
{
	class ASTNode
	{
	public:
		virtual ~ASTNode() = 0 {};
		virtual void Construct(NFA& m) = 0;
	};

	class CharRangeNode
		: public ASTNode
	{
	public:
		CharRangeNode(const std::vector<char_t>& charRange)
			: mCharRange(charRange) {}

		void Construct(NFA& m) override;
	private:
		std::vector<char_t> mCharRange;
	};

	class AltNode
		: public ASTNode
	{
	public:
		AltNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
			: mLeft(std::move(left)), mRight(std::move(right)) {}

		void Construct(NFA& m) override;
	private:
		std::unique_ptr<ASTNode> mLeft, mRight;
	};

	class ConcatNode
		: public ASTNode
	{
	public:
		ConcatNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
			: mLeft(std::move(left)), mRight(std::move(right)) {}

		void Construct(NFA& m) override;
	private:
		std::unique_ptr<ASTNode> mLeft, mRight;
	};

	class ClosureNode
		: public ASTNode
	{
	public:
		ClosureNode(std::unique_ptr<ASTNode> closure)
			: mClosure(std::move(closure)) {}

		void Construct(NFA& m) override;
	private:
		std::unique_ptr<ASTNode> mClosure;
	};
}
