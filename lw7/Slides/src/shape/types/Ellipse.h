#pragma once
#include "../Shape.h"

class Ellipse : public Shape
{
public:
	Ellipse(const Frame& frame, const Style& lineStyle, const Style& fillStyle)
		: Shape(frame, lineStyle, fillStyle)
	{
	}

	std::string GetType() const override
	{
		return "ellipse";
	}

protected:
	void DrawImpl(ICanvas& canvas) const override
	{
		canvas.DrawEllipse(m_frame.left, m_frame.top, m_frame.width, m_frame.height);
	}
};