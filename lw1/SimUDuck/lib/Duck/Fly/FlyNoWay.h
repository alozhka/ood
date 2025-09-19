#pragma once

#include "IFlyBehavior.h"

class FlyNoWay final : public IFlyBehavior
{
public:
	void Fly() override {}

	bool IsFlyable() const override
	{
		return false;
	}

	int GetFlightCount() const override
	{
		return 0;
	}
};