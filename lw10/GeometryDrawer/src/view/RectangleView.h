#pragma once

#include "ShapeView.h"

class RectangleView final : public ShapeView
{
public:
	explicit RectangleView(const QRectF& rect, QGraphicsItem* parent = nullptr)
		: ShapeView(rect, parent)
	{
	}

protected:
	void PaintShape(QPainter* painter) override
	{
		painter->drawRect(m_rect);
	}
};
