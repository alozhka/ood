#pragma once
#include "../shape/IShape.h"


class ISlide
{
public:
    virtual void AddShape(std::shared_ptr<IShape> shape) = 0;

    virtual size_t GetShapesCount() const = 0;
    virtual std::shared_ptr<IShape> GetShapeAt(size_t index) = 0;

    virtual ~ISlide() = default;
};
