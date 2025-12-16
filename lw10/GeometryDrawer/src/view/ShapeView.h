#pragma once
#include "IResizable.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>

class ShapeView : public QGraphicsObject
	, public IResizable
{
	Q_OBJECT
public:
	explicit ShapeView(const QRectF& rect, QGraphicsItem* parent = nullptr)
		: QGraphicsObject(parent)
		, m_rect(rect)
		, m_color(Qt::lightGray)
	{
		setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
	}

	QRectF boundingRect() const override
	{
		return m_rect;
	}

	QRectF GetRect() const override
	{
		return m_rect;
	}

	void SetRect(const QRectF& rect) override
	{
		m_rect = rect;
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
	{
		painter->setBrush(m_color);
		painter->setPen(Qt::NoPen);
		PaintShape(painter);

		if (isSelected())
		{
			painter->setBrush(Qt::NoBrush);
			painter->setPen(QPen(Qt::black, 1, Qt::DashDotDotLine));
			painter->drawRect(m_rect);
		}
	}

signals:
	void MovementFinished();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override
	{
		m_isMoving = false;

		QGraphicsObject::mousePressEvent(event);
		setCursor(QCursor(Qt::ClosedHandCursor));
	}

	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
	{
		QGraphicsObject::mouseMoveEvent(event);
		if (event->buttons() & Qt::LeftButton)
		{
			m_isMoving = true;
		}
	}
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
	{
		QGraphicsObject::mouseReleaseEvent(event);
		setCursor(QCursor(Qt::ArrowCursor));
		if (m_isMoving)
		{
			m_isMoving = false;
			emit MovementFinished();
		}
	}

	virtual void PaintShape(QPainter* painter) = 0;

	QRectF m_rect;
	QColor m_color;
	bool m_isMoving = false;
};
