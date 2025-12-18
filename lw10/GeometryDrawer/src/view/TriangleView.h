#pragma once
#include "ShapeView.h"

class TriangleView final : public ShapeView
{
	Q_OBJECT
public:
	TriangleView(const QRectF& rect, const MovementHandler& movementHandler, const ResizeHandler& resizeHandler, QGraphicsItem* parent = nullptr)
		: ShapeView(rect, movementHandler, resizeHandler, parent)
	{
	}

protected:
	QPainterPath shape() const override
	{
		QPainterPath path;
		path.addPolygon(PolygonFromRect(m_rect));
		return path;
	}
	void PaintShape(QPainter* painter) override
	{
		painter->drawPolygon(PolygonFromRect(m_rect));
	}

private:
	static QPolygonF PolygonFromRect(const QRectF& rect)
	{
		QPolygonF polygon;
		polygon << rect.bottomLeft()
				<< QPointF(rect.left() + rect.width() / 2, rect.top())
				<< rect.bottomRight();
		return polygon;
	}
};