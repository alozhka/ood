#pragma once
#include <format>
#include <string>

#include "IShape.h"
#include "Point.h"

class Rectangle : public IShape
{
public:
    Rectangle(int x, int y, int width, int height)
        : m_leftTop{x, y},
          m_width{width},
          m_height{height}
    {
    }

    std::string GetType() const override
    {
        return "rectangle";
    }

    std::string ListParams() const override
    {
        return std::format("x: {}, y: {}, width: {}, height: {}", m_leftTop.x, m_leftTop.y, m_width, m_height);
    }

private:
    Point m_leftTop;
    int m_width, m_height;
};
