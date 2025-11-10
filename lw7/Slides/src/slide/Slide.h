#pragma once
#include <memory>
#include <vector>

#include "ISlide.h"

class Slide final : public ISlide
{
public:
    void AddShape(std::shared_ptr<IShape> shape) override
    {
        m_shapes.push_back(shape);
    }

    size_t GetShapesCount() const override
    {
        return m_shapes.size();
    }

    std::shared_ptr<IShape> GetShapeAt(size_t index) override
    {
        EnsureValidIndex(index);
        return m_shapes[index];
    }

private:
    void EnsureValidIndex(size_t index) const
    {
        if (index >= m_shapes.size())
        {
            throw std::out_of_range("Index is out of range");
        }
    }

    std::vector<std::shared_ptr<IShape>> m_shapes;
};
