#pragma once
#include "Shape.h"

#include <QObject>

class Document : public QObject
{
	Q_OBJECT
public:
	explicit Document(QObject* parent = nullptr)
		: QObject(parent)
	{
	}

	void AddShape(Shape* shape)
	{
		shape->setParent(this);
		m_shapes.append(shape);
		emit ShapeAdded(shape);
	}

	void DeleteShape(Shape* shape)
	{
		if (m_shapes.removeOne(shape))
		{
			shape->setParent(nullptr);
			emit ShapeRemoved(shape);
			delete shape;
		}
	}

signals:
	void ShapeAdded(Shape* shape);
	void ShapeRemoved(Shape* shape);

private:
	QList<Shape*> m_shapes;
};