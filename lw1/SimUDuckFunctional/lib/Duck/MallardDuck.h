#pragma once

#include "Duck.h"

class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(
			  std::make_unique<FlyBehavior>(CreateFlyWithWings()),
			  std::make_unique<QuackBehavior>(CreateQuack()),
			  std::make_unique<DanceBehavior>(CreateWaltz()))
	{
	}

	void Display() const override
	{
		std::cout << "I'm mallard duck" << std::endl;
	}
};