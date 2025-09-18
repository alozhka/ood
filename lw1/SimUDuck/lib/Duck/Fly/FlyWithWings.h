#pragma once

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
public:
	FlyWithWings()
		: m_flightCount(0) {}

private:
	int m_flightCount;
};