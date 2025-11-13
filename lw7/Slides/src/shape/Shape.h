#pragma once
#include "IShape.h"
#include "TemplateFrame.h"

class Shape : public IShape
{
public:
	Shape(const Frame& frame, const Style& lineStyle, const Style& fillStyle)
		: m_frame(frame)
		, m_lineStyle(lineStyle)
		, m_fillStyle(fillStyle)
	{
	}

	void SetFrame(const Frame& rect) override
	{
		m_frame = rect;
	}

	void SetLineStyle(RGBAColor color) override
	{
		m_lineStyle.SetColor(color);
	}

	void SetFillStyle(RGBAColor color) override
	{
		m_fillStyle.SetColor(color);
	}

	std::optional<Frame> GetFrame() const override
	{
		return m_frame;
	}

	std::optional<Style> GetLineStyle() const override
	{
		return m_lineStyle;
	}

	std::optional<Style> GetFillStyle() const override
	{
		return m_fillStyle;
	}

protected:
	Frame m_frame{};
	Style m_lineStyle, m_fillStyle;
};
