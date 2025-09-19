#pragma once

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
public:
	FlyWithWings()
		: m_flightCount(0) {}

	void Fly() override
	{
		std::cout << "Im flying with wings! Flight #" << ++m_flightCount << std::endl;
	}

	int GetFlightCount() const override
	{
		return m_flightCount;
	}

	bool IsFlyable() const override
	{
		return true;
	}

private:
	int m_flightCount;
};