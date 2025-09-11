#pragma once

#include "../lib/Duck/Quack/IQuakBehavior.h"

class MockQuackBehavior : public IQuackBehavior
{
public:
	MockQuackBehavior() : m_quackCount(0) {}

	void Quack() override
	{
		++m_quackCount;
	}

	int GetQuackCount() const { return m_quackCount; }

	void Reset() { m_quackCount = 0; }

private:
	int m_quackCount;
};