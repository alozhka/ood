#pragma once
#include "../shape/Shape.h"


class ISlide
{
public:
    virtual void AddShape(std::shared_ptr<Shape> shape) = 0;

    virtual size_t GetShapesCount() const = 0;
    virtual std::shared_ptr<Shape> GetShapeAt(size_t index) = 0;

    virtual ~ISlide() = default;
};
