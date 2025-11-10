#pragma once
#include "IShapesFactory.h"
#include "../Rectangle.h"

class Rectangle;

class ShapesFactory final : public IShapesFactory
{
public:
    std::shared_ptr<IShape> Create(const std::string& type, std::istream& params) override
    {
        if (type == "rectangle")
        {
            return CreateRectangle(params);
        }

        throw std::invalid_argument("Cannot create unsupported shape");
    }

private:
    static std::shared_ptr<IShape> CreateRectangle(std::istream& params)
    {
        int x, y, width, height;

        if (!(params >> x >> y >> width >> height))
        {
            throw std::invalid_argument("Not all args are specified. Usage: rectangle <x> <y> <width> <height>");
        }

        return std::make_shared<Rectangle>(x, y, width, height);
    }
};
