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

void shapes::Picture::Draw(gfx::ICanvas& canvas)
{
	for (const auto& shape : m_shapes | std::views::values)
	{
		shape->Draw(canvas);
	}
}