#pragma once
#include "IResizable.h"

#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class ShapeView : public QGraphicsObject
	, public IResizable
{
	Q_OBJECT
public:
	using MovementHandler = std::function<void(ShapeView*, const QPointF& mousePos)>;

	explicit ShapeView(const QRectF& rect, QGraphicsItem* parent = nullptr)
		: QGraphicsObject(parent)
		, m_rect(rect)
		, m_color(Qt::lightGray)
		, m_capturedPos(0, 0)
	{
		setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
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

	void SetMovementHandler(const MovementHandler& handler)
	{
		m_movementHandler = handler;
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
	void MovementRequested();
	void MovementFinished();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override
	{
		QGraphicsObject::mousePressEvent(event);

		m_isMoving = false;
		m_capturedPos = event->pos();
		setCursor(QCursor(Qt::ClosedHandCursor));
	}

	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
	{
		QGraphicsObject::mouseMoveEvent(event);

		if (event->buttons() & Qt::LeftButton)
		{
			m_isMoving = true;
			QPointF currentMousePos = event->pos();
			QPointF delta = currentMousePos - m_capturedPos;
			m_movementHandler(this, delta);
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
	QPointF m_capturedPos;
	MovementHandler m_movementHandler;
};
