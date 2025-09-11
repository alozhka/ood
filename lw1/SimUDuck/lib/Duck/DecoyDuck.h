#pragma once

#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Quack/MuteQuackBehavior.h"
#include "Dance/NoDance.h"
#include <iostream>
#include <memory>

class DecoyDuck : public Duck
{
public:
	DecoyDuck()
		: Duck(std::make_unique<FlyNoWay>(), std::make_unique<MuteQuackBehavior>(), std::make_unique<NoDance>())
	{
	}

	void Display() const override
	{
		std::cout << "I'm decoy duck" << std::endl;
	}

};
