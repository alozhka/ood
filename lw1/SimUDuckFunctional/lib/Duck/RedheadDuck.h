#pragma once

#include "Duck.h"

class RedheadDuck : public Duck
{
public:
	RedheadDuck()
		: Duck(CreateFlyWithWings(), CreateQuack(), CreateMinuetDance())
	{
	}

	void Display() const override
	{
		std::cout << "I'm redhead duck" << std::endl;
	}
};