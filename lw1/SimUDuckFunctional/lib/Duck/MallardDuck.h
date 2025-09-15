#pragma once

#include "Duck.h"

class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(FlyWithWings, CreateQuackBehavior(), CreateWaltzDanceBehavior())
	{
	}

	void Display() const override
	{
		std::cout << "I'm mallard duck" << std::endl;
	}
};