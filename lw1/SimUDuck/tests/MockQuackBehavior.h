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

	bool IsQuackable() override
	{
		return true;
	}

private:
	int m_quackCounter = 0;
};