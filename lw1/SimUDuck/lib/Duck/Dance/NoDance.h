#pragma once

#include "IDanceBehavior.h"

class NoDance : public IDanceBehavior
{
public:
	void Dance() const override {}
};
