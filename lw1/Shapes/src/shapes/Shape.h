#pragma once
#include "../draw/ICanvas.h"
#include "strategy/IShapeStrategy.h"

class Shape
{
public:
	Shape(const std::string& id, gfx::Color color, std::unique_ptr<shapes::IShapeStrategy> strategy);

	void Draw(gfx::ICanvas& canvas);
	void Move(double dx, double dy);

	std::string GetId() const;

private:
	std::string m_id;
	gfx::Color m_color;
	std::unique_ptr<shapes::IShapeStrategy> m_shapeStrategy;
};