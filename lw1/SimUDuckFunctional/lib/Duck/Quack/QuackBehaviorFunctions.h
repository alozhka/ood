#pragma once
#include <functional>
#include <iostream>

struct QuackBehavior
{
	std::function<void()> Quack;
	std::function<bool()> IsQuackable;
};

inline QuackBehavior CreateMuteQuack()
{
	return {
		[] {},
		[] { return false; }
	};
}

inline QuackBehavior CreateQuack()
{
	return {
		[] { std::cout << "Quack Quack!!!\n"; },
		[] { return true; }
	};
}

inline QuackBehavior CreateSqueak()
{
	return {
		[] { std::cout << "Squeek!!!\n"; },
		[] { return true; }
	};
}