#pragma once

struct IFlyBehavior
{
	virtual void Fly() = 0;

	virtual int GetFlightCount() const = 0;
	virtual bool IsFlyable() const = 0;

	virtual ~IFlyBehavior() = default;
};