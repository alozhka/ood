#pragma once
#include <memory>

#include "../types/Rectangle.h"

class Rectangle;

class ShapesFactory
{
public:
	static std::shared_ptr<Shape> Create(const std::string& type, std::istream& params)
	{
		int left, top, width, height;
		RGBAColor lineColor, fillColor;
		if (!(params >> std::hex >> lineColor >> fillColor))
		{
			throw std::invalid_argument("Failed to read colors");
		}
		if (!(params >> std::dec >> left >> top >> width >> height))
		{
			throw std::invalid_argument("Not all args are specified");
		}

		Style fillStyle, lineStyle;
		fillStyle.SetColor(fillColor);
		lineStyle.SetColor(lineColor);
		Frame frame(left, top, width, height);

		if (type == "rectangle")
		{
			return std::make_shared<Rectangle>(frame, lineStyle, fillStyle);
		}

		throw std::invalid_argument("Cannot create unsupported shape");
	}
};
