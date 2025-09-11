#pragma once

#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
	void Fly() override {}

	void Fly(const std::function<void()>& onSpecialFlight) override {}
};
