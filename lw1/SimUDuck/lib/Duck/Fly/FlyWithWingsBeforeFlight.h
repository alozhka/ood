#pragma once

#include "IFlyBehavior.h"
#include <iostream>

// Альтернативная реализация для демонстрации лёгкого изменения поведения
// Кряканье происходит перед полётом, а не после
class FlyWithWingsBeforeFlight : public IFlyBehavior
{
public:
	FlyWithWingsBeforeFlight() : m_flightCount(0) {}

	void Fly() override
	{
		++m_flightCount;
		std::cout << "I'm flying with wings! Flight #" << m_flightCount << std::endl;
	}

	void Fly(const std::function<void()>& onSpecialFlight) override
	{
		++m_flightCount;
		
		if (m_flightCount % 2 == 0 && onSpecialFlight)
		{
			onSpecialFlight();
		}
		
		std::cout << "I'm flying with wings! Flight #" << m_flightCount << std::endl;
	}

private:
	int m_flightCount;
};
