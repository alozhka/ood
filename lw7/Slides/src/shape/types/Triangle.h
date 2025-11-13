#pragma once
#include "../Shape.h"

class Triangle : public Shape
{
public:
	Triangle(const Frame& frame, const Style& lineStyle, const Style& fillStyle)
		: Shape(frame, lineStyle, fillStyle)
	{
	}

	std::string GetType() const override
	{
		return "triangle";
	}
};