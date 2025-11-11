#pragma once
#include <format>
#include <string>

#include "../Shape.h"

class Rectangle : public Shape
{
public:
	Rectangle(const Frame& frame, Style lineStyle, Style fillStyle)
		: Shape(frame, lineStyle, fillStyle)
	{
	}

	std::string GetType() const override
	{
		return "rectangle";
	}
};
