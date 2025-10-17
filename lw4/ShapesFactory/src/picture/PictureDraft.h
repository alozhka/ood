#pragma once
#include "../shapes/Shape.h"

#include <vector>

class PictureDraft
{
public:
	using ShapePtr = std::unique_ptr<Shape>;
	using ShapesContainer = std::vector<ShapePtr>;

	void AddShape(ShapePtr&& shape)
	{
		m_shapes.push_back(std::move(shape));
	}

	size_t GetShapesCount() const
	{
		return m_shapes.size();
	}

	Shape& GetShape(size_t index) const
	{
		if (index >= GetShapesCount())
		{
			throw std::range_error("Shapes index is out of range");
		}

		return *m_shapes[index];
	}

	ShapesContainer::const_iterator begin() const
	{
		return m_shapes.begin();
	}

	ShapesContainer::const_iterator end() const
	{
		return m_shapes.end();
	}

private:
	ShapesContainer m_shapes;
};