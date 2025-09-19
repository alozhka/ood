#pragma once

#include "IDanceBehavior.h"

class NoDanceBehavior : public IDanceBehavior
{
public:
	void Dance() override {}
};
