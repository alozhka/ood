#pragma once

struct IQuackBehavior
{
	virtual void Quack() = 0;
	virtual bool IsQuackable() = 0;

	virtual ~IQuackBehavior() = default;
};