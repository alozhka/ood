#pragma once
#include <memory>

#include "../shape/ShapeGroup.h"
#include "ISlide.h"

#include <ranges>

class Slide final : public ISlide
{
public:
	void AddShape(const std::shared_ptr<IShape>& shape) override
	{
		m_shapes.push_back(shape);
	}

	void TransformShape(int index, double left, double top, double width, double height) override
	{
		std::shared_ptr<IShape> shape = GetShapeAt(index);
		Frame frame(left, top, width, height);
		shape->SetFrame(frame);
	}

	void GroupShapes(const std::vector<int>& indexes) override
	{
		if (indexes.size() < 2)
		{
			throw std::runtime_error("At least 2 shapes are required for grouping.");
		}

		std::vector<std::shared_ptr<IShape>> shapes;
		for (int index : indexes)
		{
			EnsureValidIndex(index);
			shapes.push_back(GetShapeAt(index));
		}

		auto group = std::make_shared<ShapeGroup>(shapes);

		for (int index : std::ranges::reverse_view(indexes))
		{
			m_shapes.erase(m_shapes.begin() + index);
		}

		AddShape(group);
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
