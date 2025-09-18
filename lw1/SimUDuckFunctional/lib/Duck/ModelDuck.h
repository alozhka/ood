#pragma once

#include "Duck.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(
			  std::make_unique<FlyBehavior>(CreateFlyNoWay()),
			  std::make_unique<QuackBehavior>(CreateQuack()),
			  std::make_unique<DanceBehavior>(CreateNoDance()))
	{
	}

	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}
};