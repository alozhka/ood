#pragma once
#include <format>
#include <string>

#include "../Shape.h"
#include "../Point.h"

class Rectangle : public Shape
{
public:
    Rectangle(const Frame& frame, RGBAColor fillColor, RGBAColor lineColor)
        : Shape(frame, fillColor, lineColor)
    {
    }

    std::string GetType() const override
    {
        return "rectangle";
    }

private:
    int m_width, m_height;
};
