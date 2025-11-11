#pragma once
#include "Style.h"
#include "TemplateFrame.h"

class Shape
{
public:
	Shape(const Frame& frame, const Style& lineStyle, const Style& fillStyle)
		: m_frame(frame)
		, m_lineStyle(lineStyle)
		, m_fillStyle(fillStyle)
	{
	}

	void SetFrame(const Frame& rect)
	{
		m_frame = rect;
	}

	void SetLineStyle(const Style& style)
	{
		m_lineStyle = style;
	}

	void SetFillStyle(const Style& style)
	{
		m_fillStyle = style;
	}

	// TODO: сделать рисование
	// virtual void Draw(ICanvas& canvas) const = 0;

	Frame GetFrame() const
	{
		return m_frame;
	}

	Style GetLineStyle() const
	{
		return m_lineStyle;
	}

	Style GetFillStyle() const
	{
		return m_fillStyle;
	}

	virtual std::string GetType() const = 0;

	virtual ~Shape() = default;

protected:
	Frame m_frame{};
	Style m_lineStyle, m_fillStyle;
};
