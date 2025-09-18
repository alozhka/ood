#pragma once

#include "IQuakBehavior.h"
#include <iostream>

class SqueakBehavior : public IQuackBehavior
{
public:
	void Quack() override
	{
		std::cout << "Squeek!!!" << std::endl;
	}

	bool IsQuackable() override
	{
		return true;
	}
};