#pragma once
#include <QObject>
#include <QRectF>
#include <QUuid>

class Shape : public QObject
{
	Q_OBJECT
public:
	enum class Type
	{
		Rectangle = 0,
		Triangle = 1,
		Ellipse = 2,
	};

	explicit Shape(Type type, const QRectF& rect, QObject* parent = nullptr)
		: QObject(parent)
		, m_id(QUuid::createUuid())
		, m_rect(rect)
		, m_type(type)
	{
	}

	QUuid GetId() const
	{
		return m_id;
	}

	QRectF GetRect() const
	{
		return m_rect;
	}

	Type GetType() const
	{
		return m_type;
	}

	void SetPosition(const QPointF& p)
	{
		m_rect.setTopLeft(p);
	}

signals:
	void GeometryChanged(const QRectF& rect);

private:
	QUuid m_id;
	QRectF m_rect;
	Type m_type;
};