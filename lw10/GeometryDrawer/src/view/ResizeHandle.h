#pragma once
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QWidget>

enum class HandleType
{
	TopLeft,
	Left,
	BottomLeft,
	Bottom,
	BottomRight,
	Right,
	TopRight,
	Top,
};

class ResizeHandle : public QGraphicsRectItem
{
public:
	using DragHandler = std::function<void(ResizeHandle*, const QPointF&)>;

	explicit ResizeHandle(HandleType type, QGraphicsItem* parent = nullptr)
		: QGraphicsRectItem(-4, -4, 8, 8, parent)
		, m_type(type)
	{
		setZValue(100);
		setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
		setAcceptHoverEvents(true);
		setBrush(QBrush(Qt::white));
		setPen(QPen(Qt::black));
	}

	void SetDragHandler(DragHandler handler)
	{
		m_dragHandler = handler;
	}

	HandleType GetType() const
	{
		return m_type;
	}

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override
	{
		setCursor(CursorForType(m_type));
	}

	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override
	{
		unsetCursor();
	}

	void mousePressEvent(QGraphicsSceneMouseEvent* event) override
	{
		event->accept();
	}

	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
	{
		m_dragHandler(this, mapToParent(event->pos()));
	}

private:
	static Qt::CursorShape CursorForType(HandleType type)
	{
		switch (type)
		{
		case HandleType::TopLeft:
		case HandleType::BottomRight:
			return Qt::SizeFDiagCursor;
		case HandleType::TopRight:
		case HandleType::BottomLeft:
			return Qt::SizeBDiagCursor;
		case HandleType::Top:
		case HandleType::Bottom:
			return Qt::SizeVerCursor;
		case HandleType::Left:
		case HandleType::Right:
			return Qt::SizeHorCursor;
		default:
			throw std::runtime_error("Unknown cursor type");
		}
	}

	DragHandler m_dragHandler;
	HandleType m_type;
};