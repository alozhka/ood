#pragma once

struct IFlyBehavior
{
	virtual void Fly() = 0;

	virtual int GetFlyCount() = 0;
	virtual bool IsFlyable() = 0;

	virtual ~IFlyBehavior() = default;
};