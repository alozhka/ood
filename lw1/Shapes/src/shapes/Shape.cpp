#include "Shape.h"

#include <cassert>

Shape::Shape(const std::string& id, gfx::Color color, std::unique_ptr<shapes::IShapeStrategy> strategy)
	: m_id(id)
	, m_color(color)
	, m_shapeStrategy(std::move(strategy))
{
}

void Shape::Draw(gfx::ICanvas& canvas)
{
	canvas.SetColor(m_color);
	m_shapeStrategy->Draw(canvas);
}

void Shape::Move(double dx, double dy)
{
	m_shapeStrategy->Move(dx, dy);
}

std::string Shape::GetId() const
{
	return m_id;
}

std::string Shape::GetDescription() const
{
	return m_shapeStrategy->GetType()
		+ " " + m_id
		+ " " + m_color.ToString()
		+ " " + m_shapeStrategy->GetDescription();
}