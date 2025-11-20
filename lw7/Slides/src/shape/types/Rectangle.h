#pragma once
#include <format>
#include <string>

#include "../Shape.h"

class Rectangle : public Shape
{
public:
	Rectangle(const Frame& frame, Style lineStyle, Style fillStyle)
		: Shape(frame, lineStyle, fillStyle)
	{
	}

	std::string GetType() const override
	{
		return "rectangle";
	}

	std::shared_ptr<IShape> Clone() const override
	{
		return std::make_shared<Rectangle>(m_frame, m_lineStyle, m_fillStyle);
	}

protected:
	void DrawImpl(ICanvas& canvas) const override
	{
		canvas.MoveTo(m_frame.left, m_frame.top);
		canvas.LineTo(m_frame.left + m_frame.width, m_frame.top);
		canvas.LineTo(m_frame.left + m_frame.width, m_frame.top + m_frame.height);
		canvas.LineTo(m_frame.left, m_frame.top + m_frame.height);
		canvas.LineTo(m_frame.left, m_frame.top);
	}
};
