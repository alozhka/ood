#pragma once
#include "../Shape.h"

class Ellipse : public Shape
{
public:
	Ellipse(const Frame& frame, const Style& lineStyle, const Style& fillStyle)
		: Shape(frame, lineStyle, fillStyle)
	{
	}

	std::string GetType() const override
	{
		return "ellipse";
	}
};