#pragma once

#include "IQuakBehavior.h"

class MuteQuackBehavior : public IQuackBehavior
{
public:
	void Quack() override {}
};
