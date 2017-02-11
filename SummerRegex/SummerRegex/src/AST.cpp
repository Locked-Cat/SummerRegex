#include "..\include\AST.h"
#include "..\include\\Parser.h"

using namespace summer;

NFA CharSetNode::Construct()
{
	NFA m;

	if (!mExclude)
	{
		for (auto c : mCharSet)
		{
			m.InsertEdge(m.start, m.end, c);
		}
	}
	else
	{
		for (auto c : Parser::sLegalCharSet)
		{
			if (mCharSet.find(c) == mCharSet.end())
			{
				m.InsertEdge(m.start, m.end, c);
			}
		}
	}

	return m;
}

NFA AltNode::Construct()
{
	if (mLeft != nullptr && mRight != nullptr)
	{
		auto left = mLeft->Construct();
		auto ls = left.start;
		auto le = left.end;

		auto right = mRight->Construct();
		auto rs = right.start;
		auto re = right.end;

		auto m = Merge(left, right);

		m.InsertEdge(m.start, ls, NFA::sEpsilon);
		m.InsertEdge(m.start, rs, NFA::sEpsilon);
		m.InsertEdge(le, m.end, NFA::sEpsilon);
		m.InsertEdge(re, m.end, NFA::sEpsilon);

		return m;
	}
	else
	{
		if (mLeft != nullptr)
		{
			return mLeft->Construct();
		}
		else
		{
			if (mRight != nullptr)
			{
				return mRight->Construct();
			}
		}

		return NFA();
	}
}

NFA ConcatNode::Construct()
{
	if (mLeft != nullptr && mRight != nullptr)
	{
		auto left = mLeft->Construct();
		auto ls = left.start;
		auto le = left.end;

		auto right = mRight->Construct();
		auto rs = right.start;
		auto re = right.end;

		auto m = Merge(left, right);

		m.InsertEdge(le, rs, NFA::sEpsilon);
		m.InsertEdge(m.start, ls, NFA::sEpsilon);
		m.InsertEdge(re, m.end, NFA::sEpsilon);

		return m;
	}
	else
	{
		if (mLeft != nullptr)
		{
			return mLeft->Construct();
		}
		else
		{
			if (mRight != nullptr)
			{
				return mRight->Construct();
			}

			return NFA();
		}
	}
}

NFA LoopNode::Construct()
{
	auto body = mBody->Construct();

	if (mMin > 0)
	{
		NFA prefix;
		prefix.InsertEdge(prefix.start, prefix.end, NFA::sEpsilon);

		for (auto i = 0; i < mMin; ++i)
		{
			auto ps = prefix.start;
			auto pe = prefix.end;

			auto copy = body.Clone();
			auto cs = copy.start;
			auto ce = copy.end;

			prefix = Merge(prefix, copy);
			prefix.InsertEdge(prefix.start, ps, NFA::sEpsilon);
			prefix.InsertEdge(pe, cs, NFA::sEpsilon);
			prefix.InsertEdge(ce, prefix.end, NFA::sEpsilon);
		}

		if (mMax == -1)
		{
			auto start = body.InsertStatus();
			auto end = body.InsertStatus();

			body.InsertEdge(start, body.start, NFA::sEpsilon);
			body.InsertEdge(body.end, end, NFA::sEpsilon);
			body.InsertEdge(body.end, body.start, NFA::sEpsilon);
			body.InsertEdge(start, end, NFA::sEpsilon);


			body.start = start;
			body.end = end;

			auto ps = prefix.start;
			auto pe = prefix.end;
			auto bs = body.start;
			auto be = body.end;

			prefix = Merge(prefix, body);
			prefix.InsertEdge(prefix.start, ps, NFA::sEpsilon);
			prefix.InsertEdge(pe, bs, NFA::sEpsilon);
			prefix.InsertEdge(be, prefix.end, NFA::sEpsilon);

			return prefix;
		}
		else
		{
			mMin = 0;
			mMax -= mMin;

			auto suffix = Aux(body);

			auto ps = prefix.start;
			auto pe = prefix.end;
			auto ss = suffix.start;
			auto se = suffix.end;

			prefix = Merge(prefix, suffix);
			prefix.InsertEdge(prefix.start, ps, NFA::sEpsilon);
			prefix.InsertEdge(pe, ss, NFA::sEpsilon);
			prefix.InsertEdge(se, prefix.end, NFA::sEpsilon);

			return prefix;
		}
	}
	else
	{
		if (mMax == -1)
		{
			auto start = body.InsertStatus();
			auto end = body.InsertStatus();

			body.InsertEdge(start, body.start, NFA::sEpsilon);
			body.InsertEdge(body.end, end, NFA::sEpsilon);
			body.InsertEdge(body.end, body.start, NFA::sEpsilon);
			body.InsertEdge(start, end, NFA::sEpsilon);

			body.start = start;
			body.end = end;

			return body;
		}
		else
		{
			return Aux(body);
		}
	}
}

NFA LoopNode::Aux(NFA& body)
{
	if (mMax == 0)
	{
		NFA m;
		m.InsertEdge(m.start, m.end, NFA::sEpsilon);
		return m;
	}
	else
	{
		NFA prefix;
		prefix.InsertEdge(prefix.start, prefix.end, NFA::sEpsilon);

		for (auto i = 0; i < mMax - 1; ++i)
		{
			auto ps = prefix.start;
			auto pe = prefix.end;

			auto copy = body.Clone();
			auto cs = copy.start;
			auto ce = copy.end;

			prefix = Merge(prefix, copy);
			prefix.InsertEdge(prefix.start, ps, NFA::sEpsilon);
			prefix.InsertEdge(pe, cs, NFA::sEpsilon);
			prefix.InsertEdge(ce, prefix.end, NFA::sEpsilon);
			prefix.InsertEdge(prefix.start, prefix.end, NFA::sEpsilon);

		}

		auto ps = prefix.start;
		auto pe = prefix.end;
		auto bs = body.start;
		auto be = body.end;

		prefix = Merge(prefix, body);

		prefix.InsertEdge(prefix.start, ps, NFA::sEpsilon);
		prefix.InsertEdge(pe, bs, NFA::sEpsilon);
		prefix.InsertEdge(be, prefix.end, NFA::sEpsilon);
		prefix.InsertEdge(prefix.start, prefix.end, NFA::sEpsilon);

		return prefix;
	}
}
