#pragma once
#include "ShapeView.h"

class EllipseView final : public ShapeView
{
public:
	explicit EllipseView(const QRectF& rect, QGraphicsItem* parent = nullptr)
		: ShapeView(rect, parent)
	{
	}

protected:
	QPainterPath shape() const override
	{
		QPainterPath path;
		path.addEllipse(m_rect);
		return path;
	}
	void PaintShape(QPainter* painter) override
	{
		painter->drawEllipse(m_rect);
	}
};
