#pragma once

#include <functional>
#include <iostream>

using DanceFunction = std::function<void()>;

inline DanceFunction CreateNoDanceBehavior()
{
	return [] {};
}

inline DanceFunction CreateWaltzDanceBehavior()
{
	return [] {
		std::cout << "I'm dancing waltz" << std::endl;
	};
}

// Функция для создания поведения "танцует менуэт"
inline DanceFunction CreateMinuetDanceBehavior()
{
	return [] {
		std::cout << "I'm dancing minuet" << std::endl;
	};
}
