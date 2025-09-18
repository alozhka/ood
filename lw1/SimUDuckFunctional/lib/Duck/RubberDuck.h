#pragma once

#include "Duck.h"
#include <iostream>

class RubberDuck : public Duck
{
public:
	RubberDuck()
		: Duck(
			  std::make_unique<FlyBehavior>(CreateFlyNoWay()),
			  std::make_unique<QuackBehavior>(CreateSqueak()),
			  std::make_unique<DanceBehavior>(CreateNoDance()))
	{
	}

	void Display() const override
	{
		std::cout << "I'm rubber duck" << std::endl;
	}
};