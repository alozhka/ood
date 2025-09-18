#pragma once

#include "IQuakBehavior.h"
#include <iostream>

class QuackBehavior : public IQuackBehavior
{
public:
	void Quack() override
	{
		std::cout << "Quack Quack!!!" << std::endl;
	}

	bool IsQuackable() override
	{
		return true;
	}
};