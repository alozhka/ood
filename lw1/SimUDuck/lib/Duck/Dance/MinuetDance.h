#pragma once

#include "IDanceBehavior.h"
#include <iostream>

class MinuetDance : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dancing minuet" << std::endl;
	}
};
