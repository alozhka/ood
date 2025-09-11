#pragma once

#include "IDanceBehavior.h"
#include <iostream>

class WaltzDance : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dancing waltz" << std::endl;
	}
};
