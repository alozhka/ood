#pragma once
#include "../types/Rectangle.h"

class Rectangle;

class ShapesFactory
{
public:
    static std::shared_ptr<Shape> Create(const std::string& type, std::istream& params)
    {
        int left, top, width, height;
        RGBAColor fillColor, lineColor;
        if (!(params >> std::hex >> fillColor >> lineColor))
        {
            throw std::invalid_argument("Failed to read colors");
        }
        if (!(params >> left >> top >> width >> height))
        {
            throw std::invalid_argument("Not all args are specified");
        }

        Frame frame(left, top, width, height);

        if (type == "rectangle")
        {
            return std::make_shared<Rectangle>(frame, fillColor, lineColor);
        }

        throw std::invalid_argument("Cannot create unsupported shape");
    }
};
