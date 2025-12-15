#pragma once
#include "Shape.h"

#include <QHash>
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
		m_shapesMap.insert(shape->GetId(), shape);
		emit ShapeAdded(shape);
	}

	void RemoveShapes(const QList<QUuid>& ids)
	{
		QList<QUuid> removedShapeIds;

		for (const QUuid& id : ids)
		{
			auto it = m_shapesMap.find(id);
			if (it != m_shapesMap.end())
			{
				removedShapeIds.append(it.key());
				m_shapesMap.erase(it);
			}
		}

		if (!removedShapeIds.isEmpty())
		{
			emit ShapesRemoved(removedShapeIds);
		}
	}

signals:
	void ShapeAdded(Shape* shape);
	void ShapesRemoved(const QList<QUuid>& ids);

private:
	QList<Shape*> m_shapes;
	QHash<QUuid, Shape*> m_shapesMap;
};