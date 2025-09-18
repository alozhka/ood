#pragma once

#include "Duck.h"

class RedheadDuck : public Duck
{
public:
	RedheadDuck()
		: Duck(
			  std::make_unique<FlyBehavior>(CreateFlyWithWings()),
			  std::make_unique<QuackBehavior>(CreateQuack()),
			  std::make_unique<DanceBehavior>(CreateMinuet()))
	{
	}

	void Display() const override
	{
		std::cout << "I'm redhead duck" << std::endl;
	}
};