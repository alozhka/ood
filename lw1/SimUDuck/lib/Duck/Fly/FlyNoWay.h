#pragma once

#include "IFlyBehavior.h"

class FlyNoWay final : public IFlyBehavior
{
public:
	void Fly() override {}

	int GetFlyCount() override
	{
		return 0;
	}
};