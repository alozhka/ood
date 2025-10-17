#pragma once
#include "Shape.h"

class Ellipse : public Shape
{
public:
	explicit Ellipse(Color color, Point center, double rx, double ry)
		: Shape(color)
		, m_center(center)
		, m_rx(rx)
		, m_ry(ry)
	{
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());
		canvas.DrawEllipse(m_center.x, m_center.y, m_rx, m_ry);
	}

	Point GetCenter() const
	{
		return m_center;
	}

	double GetHorizontalRadius() const
	{
		return m_rx;
	}

	double GetVerticalRadius() const
	{
		return m_ry;
	}

private:
	Point m_center;
	double m_rx, m_ry;
};