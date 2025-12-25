#pragma once
#include "../view/ShapeView.h"

#include <QGraphicsScene>
#include <QPointF>
#include <QRectF>

class ShapeViewManipulator
{
public:
	static void MoveShapeWithBounds(
		ShapeView* shapeView,
		const QPointF& delta,
		const QRectF& sceneBounds)
	{
		QPointF targetPos = shapeView->pos() + delta;
		QRectF itemRect = shapeView->boundingRect();

		qreal minX = sceneBounds.left() - itemRect.left();
		qreal maxX = sceneBounds.right() - itemRect.right();
		qreal minY = sceneBounds.top() - itemRect.top();
		qreal maxY = sceneBounds.bottom() - itemRect.bottom();

		qreal clampedX = qBound(minX, targetPos.x(), maxX);
		qreal clampedY = qBound(minY, targetPos.y(), maxY);

		shapeView->setPos(clampedX, clampedY);
	}

	static void ResizeShapeWithBounds(
		ShapeView* shapeView,
		HandleType type,
		const QPointF& mousePos,
		const QRectF& sceneBounds,
		qreal minShapeSize)
	{
		QPointF sceneMousePos = shapeView->mapToScene(mousePos);
		QRectF currentGeo = shapeView->mapRectToScene(shapeView->GetRect());

		qreal newLeft = currentGeo.left();
		qreal newRight = currentGeo.right();
		qreal newTop = currentGeo.top();
		qreal newBottom = currentGeo.bottom();

		if (type == HandleType::Left || type == HandleType::TopLeft || type == HandleType::BottomLeft)
		{
			qreal minLimit = sceneBounds.left();
			qreal maxLimit = currentGeo.right() - minShapeSize;
			newLeft = qBound(minLimit, sceneMousePos.x(), maxLimit);
		}
		else if (type == HandleType::Right || type == HandleType::TopRight || type == HandleType::BottomRight)
		{
			qreal minLimit = currentGeo.left() + minShapeSize;
			qreal maxLimit = sceneBounds.right();
			newRight = qBound(minLimit, sceneMousePos.x(), maxLimit);
		}

		if (type == HandleType::Top || type == HandleType::TopLeft || type == HandleType::TopRight)
		{
			qreal minLimit = sceneBounds.top();
			qreal maxLimit = currentGeo.bottom() - minShapeSize;
			newTop = qBound(minLimit, sceneMousePos.y(), maxLimit);
		}
		else if (type == HandleType::Bottom || type == HandleType::BottomLeft || type == HandleType::BottomRight)
		{
			qreal minLimit = currentGeo.top() + minShapeSize;
			qreal maxLimit = sceneBounds.bottom();
			newBottom = qBound(minLimit, sceneMousePos.y(), maxLimit);
		}

		shapeView->setPos(newLeft, newTop);
		shapeView->SetRect(QRectF(0, 0, newRight - newLeft, newBottom - newTop));
	}
};