#pragma once

#include "Duck/Duck.h"

inline void DrawDuck(const Duck& duck)
{
	duck.Display();
}

inline void PlayWithDuck(const Duck& duck)
{
	DrawDuck(duck);
	duck.Quack();
	duck.Fly();
	duck.Dance();
	std::cout << std::endl;
}
