#pragma once
#include "../lib/Duck/Fly/IFlyBehavior.h"

class MockFlyBehavior final : public IFlyBehavior
{
public:
	void Fly() override
	{
		++m_flyCounter;
	}

	int GetFlyCount() override
	{
		return m_flyCounter;
	}

	bool IsFlyable() override
	{
		return true;
	}

private:
	int m_flyCounter = 0;
};