#pragma once
#include "../Shape.h"

class Triangle : public Shape
{
public:
	Triangle(const Frame& frame, const Style& lineStyle, const Style& fillStyle)
		: Shape(frame, lineStyle, fillStyle)
	{
	}

	std::string GetType() const override
	{
		return "triangle";
	}

protected:
	void DrawImpl(ICanvas& canvas) const override
	{
		double x1 = m_frame.left;
		double y1 = m_frame.top + m_frame.height;
		double x2 = m_frame.left + (m_frame.width / 2);
		double y2 = m_frame.top;
		double x3 = m_frame.left + m_frame.width;
		double y3 = m_frame.top + m_frame.height;

		canvas.MoveTo(x1, y1);
		canvas.LineTo(x2, y2);
		canvas.LineTo(x3, y3);
		canvas.LineTo(x1, y1);
	}
};