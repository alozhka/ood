#pragma once

#include <functional>
#include <iostream>

using FlyCallback = std::function<void()>;
using FlyFunction = std::function<void(const FlyCallback&)>;

FlyFunction CreateFlyWithWings()
{
	return [flightCount = 0](const std::function<void()>& onSpecialFlight) mutable {
		++flightCount;
		std::cout << "I'm flying with wings! Flight #" << flightCount << std::endl;

		if (flightCount % 2 == 0)
		{
			onSpecialFlight();
		}
	};
}

FlyFunction CreateFlyNoWay()
{
	return FlyFunction([](const FlyCallback& onSpecialFlight) {});
}