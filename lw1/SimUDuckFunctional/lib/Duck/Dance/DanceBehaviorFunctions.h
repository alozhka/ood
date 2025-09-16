#pragma once

#include <functional>
#include <iostream>

using DanceFunction = std::function<void()>;

inline DanceFunction CreateNoDance()
{
	return [] {};
}

inline DanceFunction CreateWaltzDance()
{
	return [] {
		std::cout << "I'm dancing waltz" << std::endl;
	};
}

// Функция для создания поведения "танцует менуэт"
inline DanceFunction CreateMinuetDance()
{
	return [] {
		std::cout << "I'm dancing minuet" << std::endl;
	};
}
