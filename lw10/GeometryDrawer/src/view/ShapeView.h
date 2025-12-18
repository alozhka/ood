#pragma once
#include "IResizable.h"
#include "ResizeHandle.h"

#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class ShapeView : public QGraphicsObject
	, public IResizable
{
	Q_OBJECT
public:
	using MovementHandler = std::function<void(ShapeView*, const QPointF&)>;
	using ResizeHandler = std::function<void(ShapeView*, HandleType, const QPointF&)>;

	explicit ShapeView(
		const QRectF& rect,
		const MovementHandler& movementHandler,
		const ResizeHandler& resizeHandler,
		QGraphicsItem* parent = nullptr)
		: QGraphicsObject(parent)
		, m_rect(rect)
		, m_color(Qt::lightGray)
		, m_capturedPos(0, 0)
		, m_movementHandler(movementHandler)
		, m_resizeHandler(resizeHandler)
	{
		setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
		CreateHandles();
		UpdateHandlesPosition();
		ShowHandles(false);
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
		if (m_rect == rect)
		{
			return;
		}
		prepareGeometryChange();
		m_rect = rect;
		UpdateHandlesPosition();
		update();
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

	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override
	{
		if (change == ItemSelectedChange)
		{
			ShowHandles(value.toBool());
		}
		return QGraphicsObject::itemChange(change, value);
	}

	virtual void PaintShape(QPainter* painter) = 0;

	QRectF m_rect;
	QColor m_color;

private:
	void UpdateHandlesPosition()
	{
		GetHandle(HandleType::TopLeft)->setPos(m_rect.left(), m_rect.top());
		GetHandle(HandleType::Left)->setPos(m_rect.left(), m_rect.center().y());
		GetHandle(HandleType::BottomLeft)->setPos(m_rect.left(), m_rect.bottom());
		GetHandle(HandleType::Bottom)->setPos(m_rect.center().x(), m_rect.bottom());
		GetHandle(HandleType::BottomRight)->setPos(m_rect.right(), m_rect.bottom());
		GetHandle(HandleType::Right)->setPos(m_rect.right(), m_rect.center().y());
		GetHandle(HandleType::TopRight)->setPos(m_rect.right(), m_rect.top());
		GetHandle(HandleType::Top)->setPos(m_rect.center().x(), m_rect.top());
	}

	void ShowHandles(bool show)
	{
		for (ResizeHandle* h : m_resizeHandles)
		{
			h->setVisible(show);
		}
	}

	ResizeHandle* GetHandle(HandleType t)
	{
		for (auto handle : m_resizeHandles)
		{
			if (handle->GetType() == t)
			{
				return handle;
			}
		}

		return nullptr;
	}

	void CreateHandles()
	{
		const QList handleTypes = {
			HandleType::TopLeft, HandleType::Left, HandleType::BottomLeft, HandleType::Bottom,
			HandleType::BottomRight, HandleType::Right, HandleType::TopRight, HandleType::Top
		};

		for (auto type : handleTypes)
		{
			auto dragHandler = [this](ResizeHandle* handle, const QPointF& pos) {
				m_resizeHandler(this, handle->GetType(), pos);
			};
			ResizeHandle* resizeHandle = new ResizeHandle(type, dragHandler, this);
			m_resizeHandles.append(resizeHandle);
		}
	}

	bool m_isMoving = false;
	QPointF m_capturedPos;
	QList<ResizeHandle*> m_resizeHandles;

	MovementHandler m_movementHandler;
	ResizeHandler m_resizeHandler;
};
