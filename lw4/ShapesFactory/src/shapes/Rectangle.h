#pragma once
#include "Shape.h"

class Rectangle : public Shape
{
public:
	explicit Rectangle(Color color, Point leftTop, double width, double height)
		: Shape(color)
		, m_leftTop(leftTop)
		, m_width(width)
		, m_height(height)
	{
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());
		canvas.DrawLine(m_leftTop.x, m_leftTop.y, m_leftTop.x + m_width, m_leftTop.y);
		canvas.DrawLine(m_leftTop.x + m_width, m_leftTop.y, m_leftTop.x + m_width, m_leftTop.y + m_height);
		canvas.DrawLine(m_leftTop.x + m_width, m_leftTop.y + m_height, m_leftTop.x, m_leftTop.y + m_height);
		canvas.DrawLine(m_leftTop.x, m_leftTop.y + m_height, m_leftTop.x, m_leftTop.y);
	}

	Point GetLeftTop() const
	{
		return m_leftTop;
	}

	double GetWidth() const
	{
		return m_width;
	}

	double GetHeight() const
	{
		return m_height;
	}

private:
	Point m_leftTop;
	double m_width, m_height;
};