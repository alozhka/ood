#pragma once

#include "Duck.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(CreateFlyWithWings(), CreateQuack(), CreateNoDance())
	{
	}

	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}
};
