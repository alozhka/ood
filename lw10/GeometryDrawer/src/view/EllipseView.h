#pragma once
#include "ShapeView.h"

class EllipseView final : public ShapeView
{
	Q_OBJECT
public:
	EllipseView(const QRectF& rect, const MovementHandler& movementHandler, const ResizeHandler& resizeHandler, QGraphicsItem* parent = nullptr)
		: ShapeView(rect, movementHandler, resizeHandler, parent)
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
