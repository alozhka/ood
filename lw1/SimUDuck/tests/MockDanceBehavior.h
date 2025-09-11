#pragma once

#include "../lib/Duck/Dance/IDanceBehavior.h"
#include <string>

class MockDanceBehavior : public IDanceBehavior
{
public:
	MockDanceBehavior()
		: m_wasCalled(false)
	{
	}

	void Dance() const override
	{
		m_wasCalled = true;
	}

	bool WasCalled() const
	{
		return m_wasCalled;
	}

private:
	mutable bool m_wasCalled;
};
