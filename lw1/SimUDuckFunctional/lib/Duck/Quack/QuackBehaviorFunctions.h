#pragma once

#include <functional>
#include <iostream>

using QuackFunction = std::function<void()>;

inline QuackFunction CreateQuack()
{
	return []() -> void {
		std::cout << "Quack" << std::endl;
	};
}

inline QuackFunction CreateSqueak()
{
	return [] {
		std::cout << "Squeak" << std::endl;
	};
}

inline QuackFunction CreateMuteQuack()
{
	return [] {};
}
