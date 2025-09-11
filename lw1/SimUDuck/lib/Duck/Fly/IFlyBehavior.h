#pragma once

#include <functional>

struct IFlyBehavior
{
	virtual void Fly() = 0;
	virtual void Fly(const std::function<void()>& onSpecialFlight) = 0;

	virtual ~IFlyBehavior() = default;
};