#pragma once

#include "../lib/Duck/Duck.h"
#include "../lib/Duck/Fly/FlyNoWay.h"
#include "../lib/Duck/Quack/MuteQuackBehavior.h"

class TestDuck : public Duck
{
public:
	TestDuck(std::unique_ptr<IDanceBehavior>&& danceBehavior)
		: Duck(std::make_unique<FlyNoWay>(), std::make_unique<MuteQuackBehavior>(), std::move(danceBehavior))
	{
	}

	void Display() const override
	{
	}
};
