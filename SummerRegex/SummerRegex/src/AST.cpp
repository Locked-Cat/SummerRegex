#include "..\include\AST.h"

using namespace summer;

void CharRangeNode::Construct(NFA& m)
{
	auto start = m.AddStatus();
	auto end = m.AddStatus();

	m.Start() = start;
	m.End() = end;

	for (auto c : mCharRange)
	{
		m.AddEdge(start, end, c);
	}
}

void AltNode::Construct(NFA& m)
{
	if (mLeft != nullptr && mRight != nullptr)
	{
		mLeft->Construct(m);
		auto lStart = m.Start();
		auto lEnd = m.End();

		mRight->Construct(m);
		auto rStart = m.Start();
		auto rEnd = m.End();

		auto start = m.AddStatus();
		auto end = m.AddStatus();

		m.Start() = start;
		m.End() = end;

		m.AddEdge(start, lStart, NFA::sEpsilon);
		m.AddEdge(start, rStart, NFA::sEpsilon);
		m.AddEdge(lEnd, end, NFA::sEpsilon);
		m.AddEdge(rEnd, end, NFA::sEpsilon);
	}
	else
	{
		if (mLeft != nullptr)
		{
			mLeft->Construct(m);
		}
		else
		{
			if (mRight != nullptr)
			{
				mRight->Construct(m);
			}
		}
	}
}

void ConcatNode::Construct(NFA& m)
{
	if (mLeft != nullptr && mRight != nullptr)
	{
		mLeft->Construct(m);
		auto lStart = m.Start();
		auto lEnd = m.End();

		mRight->Construct(m);
		auto rStart = m.Start();
		auto rEnd = m.End();

		m.AddEdge(lEnd, rStart, NFA::sEpsilon);
		m.Start() = lStart;
		m.End() = rEnd;
	}
	else
	{
		if (mLeft != nullptr)
		{
			return mLeft->Construct(m);
		}
		else
		{
			if (mRight != nullptr)
			{
				mRight->Construct(m);
			}
		}
	}
}

void ClosureNode::Construct(NFA& m)
{
	mClosure->Construct(m);
	m.AddEdge(m.End(), m.Start(), NFA::sEpsilon);

	auto start = m.AddStatus();
	m.AddEdge(start, m.Start(), NFA::sEpsilon);
	auto end = m.AddStatus();
	m.AddEdge(m.End(), end, NFA::sEpsilon);
	m.AddEdge(start, end, NFA::sEpsilon);

	m.Start() = start;
	m.End() = end;
}