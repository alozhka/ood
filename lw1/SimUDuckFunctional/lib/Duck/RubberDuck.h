#pragma once

#include "Duck.h"

class RubberDuck : public Duck
{
public:
	RubberDuck()
		: Duck(FlyNoWay, CreateSqueakBehavior(), CreateNoDanceBehavior())
	{
	}

	void Display() const override
	{
		std::cout << "I'm rubber duck" << std::endl;
	}
};