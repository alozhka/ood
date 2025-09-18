#pragma once
#include "../lib/Duck/Fly/IFlyBehavior.h"

class MockFlyBehavior final : public IFlyBehavior
{
public:
	explicit MockFlyBehavior(bool flyable = true)
		: m_flyable(flyable) {}

	void Fly() override
	{
		++m_flyCounter;
	}

	int GetFlyCount() override
	{
		return m_flyCounter;
	}

private:
	int m_flyCounter = 0;
	bool m_flyable;
};