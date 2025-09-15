#pragma once

#include <functional>
#include <iostream>

using FlyCallback = std::function<void()>;
using FlyFunction = std::function<void(const FlyCallback&)>;

inline FlyFunction FlyWithWings = [flightCount = 0](const std::function<void()>& onSpecialFlight) mutable {
	++flightCount;
	std::cout << "I'm flying with wings! Flight #" << flightCount << std::endl;

	if (flightCount % 2 == 0)
	{
		onSpecialFlight();
	}
};

inline FlyFunction FlyNoWay = [](const std::function<void()>& onSpecialFlight) {};

FlyFunction CreateFlyNoWay()
{
	return FlyFunction([](const FlyCallback& onSpecialFlight) {});
}