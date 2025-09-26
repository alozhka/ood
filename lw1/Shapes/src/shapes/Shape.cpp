#include "Shape.h"

#include <cassert>

Shape::Shape(const std::string& id, gfx::Color color, std::unique_ptr<shapes::IShapeStrategy>&& strategy)
	: m_id(id)
	, m_color(color)
{
	SetStrategy(std::move(strategy));
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

void Shape::SetStrategy(std::unique_ptr<shapes::IShapeStrategy>&& strategy)
{
	assert(strategy);
	m_shapeStrategy = std::move(strategy);
}
void Shape::SetColor(gfx::Color color)
{
	m_color = color;
}

std::string Shape::GetId() const
{
	return m_id;
}

gfx::Color Shape::GetColor() const
{
	return m_color;
}

std::string Shape::GetDescription() const
{
	return m_shapeStrategy->GetType()
		+ " " + m_id
		+ " " + m_color.ToString()
		+ " " + m_shapeStrategy->GetDescription();
}

std::unique_ptr<Shape> Shape::Clone(const std::string& newId) const
{
	auto clonedStrategy = m_shapeStrategy->Clone();
	return std::make_unique<Shape>(newId, m_color, std::move(clonedStrategy));
}