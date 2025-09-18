#pragma once

#include "../lib/Duck/Duck.h"

class MockDuck final : public Duck
{
public:
	explicit MockDuck(
		std::unique_ptr<IQuackBehavior>&& quackBehavior,
		std::unique_ptr<IFlyBehavior>&& flyBehavior,
		std::unique_ptr<IDanceBehavior>&& danceBehavior)
		: Duck(
			std::move(flyBehavior),
			std::move(quackBehavior),
			std::move(danceBehavior)
			) {}

	void Display() const override
	{
		std::cout << "Mock duck";
	}
};