#pragma once
#include <QObject>
#include <QRectF>

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
		, m_rect(rect)
		, m_type(type)
	{
	}

	[[nodiscard]] QRectF GetRect() const
	{
		return m_rect;
	}

	[[nodiscard]] Type GetType() const
	{
		return m_type;
	}

	void SetRect(const QRectF& rect)
	{
		m_rect = rect;
	}

signals:
	void GeometryChanged(const QRectF& rect);

private:
	QRectF m_rect;
	Type m_type;
};