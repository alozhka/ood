#include "Shape.h"

#include <cassert>

Shape::Shape(const std::string& id, gfx::Color color, std::unique_ptr<shapes::IShapeStrategy> strategy)
	: m_id(id)
	, m_color(color)
	, m_shapeStrategy(std::move(strategy))
{
}

void Shape::Draw(gfx::ICanvas& canvas) const
{
	m_shapeStrategy->Draw(canvas);
}

void Shape::Move(double dx, double dy) const
{
	m_shapeStrategy->Move(dx, dy);
}

std::string Shape::GetId() const
{
	return m_id;
}