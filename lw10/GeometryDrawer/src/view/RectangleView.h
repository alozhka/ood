#pragma once

#include "ShapeView.h"

class RectangleView final : public ShapeView
{
	Q_OBJECT
public:
	RectangleView(const QRectF& rect, const MovementHandler& movementHandler, const ResizeHandler& resizeHandler, QGraphicsItem* parent = nullptr)
		: ShapeView(rect, movementHandler, resizeHandler, parent)
	{
	}

protected:
	void PaintShape(QPainter* painter) override
	{
		painter->drawRect(m_rect);
	}
};
