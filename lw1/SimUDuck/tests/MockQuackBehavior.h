#pragma once
#include "../lib/Duck/Quack/QuackBehavior.h"

class MockQuackBehavior final : public IQuackBehavior
{
public:
	void Quack() override
	{
		++m_quackCounter;
	}

	int GetQuackCount() const
	{
		return m_quackCounter;
	}

private:
	int m_quackCounter = 0;
};