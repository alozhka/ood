#pragma once

#include "../lib/Duck/Duck.h"
#include "../lib/Duck/Dance/NoDance.h"

class TestDuckWithMocks : public Duck
{
public:
	TestDuckWithMocks(std::unique_ptr<IFlyBehavior>&& flyBehavior,
					  std::unique_ptr<IQuackBehavior>&& quackBehavior)
		: Duck(std::move(flyBehavior),
			   std::move(quackBehavior),
			   std::make_unique<NoDance>())
	{
	}

	void Display() const override
	{
		// Пустая реализация для тестирования
	}
};