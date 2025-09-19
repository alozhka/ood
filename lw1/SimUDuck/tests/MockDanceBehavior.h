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

	void Dance() override
	{
		++m_callCount;
	}

	int GetCallCount() const
	{
		return m_callCount;
	}

private:
	int m_callCount;
};
