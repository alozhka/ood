#pragma once

#include "Duck.h"

class RedheadDuck : public Duck
{
public:
	RedheadDuck()
		: Duck(FlyWithWings, CreateQuackBehavior(), CreateMinuetDanceBehavior())
	{
	}

	void Display() const override
	{
		std::cout << "I'm redhead duck" << std::endl;
	}
};