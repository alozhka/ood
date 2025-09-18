#pragma once

#include "../lib/Duck/Dance/IDanceBehavior.h"
#include <string>

class MockDanceBehavior : public IDanceBehavior
{
public:
	MockDanceBehavior()
		: m_callCount(0)
	{
	}

	void Dance() const override
	{
		++m_callCount;
	}

	int GetCallCount() const
	{
		return m_callCount;
	}

private:
	mutable int m_callCount;
};
