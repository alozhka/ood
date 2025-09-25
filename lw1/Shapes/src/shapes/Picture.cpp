#include "Picture.h"

#include <ranges>

void shapes::Picture::AddShape(std::unique_ptr<Shape>&& shape)
{
	if (m_shapes.contains(shape->GetId()))
	{
		throw std::runtime_error("Cannot create shape with same ID");
	}

	std::string shapeId = shape->GetId();
	m_shapes.insert({ shapeId, std::move(shape) });
	m_shapesOrder.push_back(shapeId);
}

void shapes::Picture::DrawShape(const std::string& id, gfx::ICanvas& canvas)
{
	Shape* shape = GetShape(id);
	shape->Draw(canvas);
}

void shapes::Picture::DrawPicture(gfx::ICanvas& canvas)
{
	for (const auto& shape : m_shapes | std::views::values)
	{
		shape->Draw(canvas);
	}
}

std::vector<Shape*> shapes::Picture::ListShapes() const
{
	std::vector<Shape*> data;
	data.reserve(m_shapes.size());

	for (const std::string& m_shapeId : m_shapesOrder)
	{
		data.push_back(m_shapes.find(m_shapeId)->second.get());
	}

	return data;
}

Shape* shapes::Picture::GetShape(const std::string& id) const
{
	const auto it = m_shapes.find(id);
	if (it == m_shapes.end())
	{
		throw std::runtime_error("Cannot find shape with ID=" + id);
	}

	return it->second.get();
}