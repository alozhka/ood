#pragma once

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
public:
	FlyWithWings()
		: m_flightCount(0)
	{
	}

	void Fly() override
	{
		++m_flightCount;
		std::cout << "I'm flying with wings! Flight #" << m_flightCount << std::endl;
	}

	void Fly(const std::function<void()>& onSpecialFlight) override
	{
		++m_flightCount;
		std::cout << "I'm flying with wings! Flight #" << m_flightCount << std::endl;

		if (m_flightCount % 2 == 0 && onSpecialFlight)
		{
			onSpecialFlight();
		}
	}

private:
	int m_flightCount;
};