#pragma once

#include "Duck.h"

class DecoyDuck : public Duck
{
public:
	DecoyDuck()
		: Duck(CreateFlyNoWay(), CreateMuteQuack(), CreateNoDance())
	{
	}

	void Display() const override
	{
		std::cout << "I'm decoy duck" << std::endl;
	}

};
