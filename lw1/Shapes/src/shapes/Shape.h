#pragma once
#include "../draw/ICanvas.h"
#include "strategy/IShapeStrategy.h"

class Shape
{
public:
	Shape(int id, gfx::Color color, std::unique_ptr<shapes::IShapeStrategy> strategy);

	void Draw(gfx::ICanvas& canvas) const;
	void Move(double dx, double dy) const;

	int GetId() const;

private:
	int m_id;
	gfx::Color m_color;
	std::unique_ptr<shapes::IShapeStrategy> m_shapeStrategy;
};