#pragma once

#include "IDanceBehavior.h"
#include <iostream>

class MinuetDanceBehavior : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dancing minuet" << std::endl;
	}
};
