#pragma once
#include <functional>
#include <iostream>

using DanceBehavior = std::function<void()>;

inline DanceBehavior CreateNoDance()
{
	return [] {};
}

inline DanceBehavior CreateMinuet()
{
	return [] { std::cout << "I'm dancing minuet\n"; };
}

static DanceBehavior CreateWaltz()
{
	return [] { std::cout << "I'm dancing waltz\n"; };
}