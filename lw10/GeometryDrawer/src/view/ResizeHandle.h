#pragma once
#include <QBrush>
#include <QGraphicsRectItem>
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
		setBrush(QBrush(Qt::white));
		setPen(QPen(Qt::black));
		setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
		setAcceptHoverEvents(true);
	}

	void SetDragHandler(DragHandler handler)
	{
		m_dragHandler = handler;
	}

	HandleType GetType() const
	{
		return m_type;
	}

private:
	DragHandler m_dragHandler;
	HandleType m_type;
};