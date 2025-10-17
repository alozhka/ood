#pragma once
#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle(Color color, Point p1, Point p2, Point p3)
		: Shape(color)
		, m_p1(p1)
		, m_p2(p2)
		, m_p3(p3)
	{
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());
		canvas.DrawLine(m_p1.x, m_p1.y, m_p2.x, m_p2.y);
		canvas.DrawLine(m_p2.x, m_p2.y, m_p3.x, m_p3.y);
		canvas.DrawLine(m_p3.x, m_p3.y, m_p1.x, m_p1.y);
	}

	Point GetVertex1() const { return m_p1; }

	Point GetVertex2() const { return m_p2; }

	Point GetVertex3() const { return m_p3; }

private:
	Point m_p1, m_p2, m_p3;
};