#pragma once

#include <functional>
#include <iostream>

using QuackFunction = std::function<void()>;

inline QuackFunction CreateQuackBehavior()
{
	return []() -> void {
		std::cout << "Quack" << std::endl;
	};
}

inline QuackFunction CreateSqueakBehavior()
{
	return [] {
		std::cout << "Squeak" << std::endl;
	};
}

inline QuackFunction CreateMuteQuackBehavior()
{
	return [] {};
}
