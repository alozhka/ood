#pragma once
#include "../draw/ICanvas.h"
#include "strategy/IShapeStrategy.h"

class Shape
{
public:
	Shape(const std::string& id, gfx::Color color, std::unique_ptr<shapes::IShapeStrategy>&& strategy);

	void Draw(gfx::ICanvas& canvas);
	void Move(double dx, double dy);
	void SetStrategy(std::unique_ptr<shapes::IShapeStrategy>&& strategy);
	void SetColor(gfx::Color color);

	std::string GetId() const;
	gfx::Color GetColor() const;
	std::string GetDescription() const;

private:
	std::string m_id;
	gfx::Color m_color;
	std::unique_ptr<shapes::IShapeStrategy> m_shapeStrategy;
};