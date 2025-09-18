#pragma once
#include <functional>

struct FlyBehavior
{
	std::function<void()> Fly;
	std::function<bool()> IsFlyable;
	std::function<int()> GetFlyCount;
};

inline FlyBehavior CreateFlyNoWay()
{
	return {
		[] {},
		[] { return false; },
		[] { return 0; }
	};
}

inline FlyBehavior CreateFlyWithWings()
{
	auto flyCount = std::make_shared<int>(0);

	return {
		[flyCount] {
			std::cout << "Im flying with wings!! Flight #" << ++*flyCount << std::endl;
		},
		[] { return true; },
		[flyCount] { return *flyCount; }
	};
}