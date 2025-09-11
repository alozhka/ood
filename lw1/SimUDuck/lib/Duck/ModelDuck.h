#pragma once

#include "Dance/NoDanceBehavior.h"
#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Quack/QuackBehavior.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(std::make_unique<FlyNoWay>(), std::make_unique<QuackBehavior>(), std::make_unique<NoDanceBehavior>())
	{
	}

	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}
};
