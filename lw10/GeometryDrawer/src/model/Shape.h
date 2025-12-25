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

	explicit Shape(Type type, const QRectF& rect, int layer = 0, QObject* parent = nullptr)
		: QObject(parent)
		, m_id(QUuid::createUuid())
		, m_rect(rect)
		, m_type(type)
		, m_layer(layer)
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

	void SetGeometry(const QRectF& rect)
	{
		m_rect = rect;
	}

	int GetLayer() const
	{
		return m_layer;
	}

private:
	QUuid m_id;
	QRectF m_rect;
	Type m_type;
	int m_layer = 0;
};