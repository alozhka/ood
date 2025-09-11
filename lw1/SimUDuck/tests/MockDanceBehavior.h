#pragma once

#include "../lib/Duck/Dance/IDanceBehavior.h"
#include <string>

class MockDanceBehavior : public IDanceBehavior
{
public:
	MockDanceBehavior() : m_wasCalled(false) {}

	void Dance() const override
	{
		m_wasCalled = true;
		m_danceMessage = "Mock dance called";
	}

	bool WasCalled() const
	{
		return m_wasCalled;
	}

	std::string GetDanceMessage() const
	{
		return m_danceMessage;
	}

	void Reset() const
	{
		m_wasCalled = false;
		m_danceMessage = "";
	}

private:
	mutable bool m_wasCalled;
	mutable std::string m_danceMessage;
};
