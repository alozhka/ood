#pragma once

#include "Duck.h"

class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(CreateFlyWithWings(), CreateQuack(), CreateWaltzDance())
	{
	}

	void Display() const override
	{
		std::cout << "I'm mallard duck" << std::endl;
	}
};