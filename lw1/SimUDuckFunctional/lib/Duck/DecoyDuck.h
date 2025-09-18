#pragma once

#include "Duck.h"
#include "Quack/QuackBehaviorFunctions.h"

#include <iostream>

class DecoyDuck : public Duck
{
public:
	DecoyDuck()
		: Duck(std::make_unique<FlyBehavior>(CreateFlyNoWay()),
			  std::make_unique<QuackBehavior>(CreateMuteQuack()),
			  std::make_unique<DanceBehavior>(CreateNoDance()))
	{
	}

	void Display() const override
	{
		std::cout << "I'm decoy duck" << std::endl;
	}
};