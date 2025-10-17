#pragma once
#include "../canvas/Color.h"
#include "../canvas/ICanvas.h"

class Shape
{
public:
	explicit Shape(Color color)
		: m_color(color)
	{
	}

	virtual void Draw(ICanvas& canvas) = 0;

	Color GetColor() const { return m_color; }

	virtual ~Shape() = default;

private:
	Color m_color;
};