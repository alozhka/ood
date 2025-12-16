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

	void UpdateShapePositions(const QHash<QUuid, QPointF>& positions)
	{
		for (auto posIt = positions.begin(); posIt != positions.end(); ++posIt)
		{
			UpdateShapePosition(posIt.key(), posIt.value());
		}
	}

signals:
	void ShapeAdded(Shape* shape);
	void ShapesRemoved(const QList<QUuid>& ids);

private:
	void UpdateShapePosition(const QUuid& id, QPointF pos)
	{
		auto it = m_shapesMap.find(id);
		if (it != m_shapesMap.end())
		{
			Shape* shape = it.value();
			shape->SetPosition(pos);
		}
	}

	QHash<QUuid, Shape*> m_shapesMap;
};