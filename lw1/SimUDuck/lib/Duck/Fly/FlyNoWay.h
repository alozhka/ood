#pragma once

#include "IFlyBehavior.h"

class FlyNoWay final : public IFlyBehavior
{
public:
	void Fly() override {}

	bool IsFlyable() override
	{
		return false;
	}

	int GetFlyCount() override
	{
		return 0;
	}
};