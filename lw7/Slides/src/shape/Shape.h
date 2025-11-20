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

	void SetLineStyle(RGBAColor color, bool isEnabled) override
	{
		m_lineStyle.SetColor(color);
		m_lineStyle.Enable(isEnabled);
	}

	void SetFillStyle(RGBAColor color, bool isEnabled) override
	{
		m_fillStyle.SetColor(color);
		m_fillStyle.Enable(isEnabled);
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

	void Draw(ICanvas& canvas) const override
	{
		if (m_lineStyle.IsEnabled())
		{
			canvas.SetLineColor(m_lineStyle.GetColor());
		}
		if (m_fillStyle.IsEnabled())
		{
			canvas.BeginFill(m_fillStyle.GetColor());
		}

		DrawImpl(canvas);

		if (m_fillStyle.IsEnabled())
		{
			canvas.EndFill();
		}
	}

protected:
	virtual void DrawImpl(ICanvas& canvas) const = 0;

	Frame m_frame{};
	Style m_lineStyle, m_fillStyle;
};
