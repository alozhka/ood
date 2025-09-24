#include "Picture.h"

#include <ranges>

void shapes::Picture::AddShape(std::unique_ptr<Shape>&& shape)
{
	if (m_shapes.contains(shape->GetId()))
	{
		throw std::runtime_error("Cannot create shape with same ID");
	}

	m_shapes[shape->GetId()] = std::move(shape);
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

	for (const auto& shape : m_shapes | std::views::values)
	{
		data.push_back(shape.get());
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