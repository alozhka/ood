#pragma once
#include "Shape.h"

#include <QDebug>
#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QUndoStack>

class Document : public QObject
{
	Q_OBJECT
public:
	explicit Document(QObject* parent = nullptr)
		: QObject(parent)
	{
	}

	void AddShapes(const QList<QSharedPointer<Shape>>& shapes)
	{
		for (const auto& shape : shapes)
		{
			m_shapesMap.insert(shape->GetId(), shape);
			qDebug() << "Added shape" << shape->GetId();
		}
		emit ShapesAdded(shapes);
	}

	QList<QSharedPointer<Shape>> RemoveShapes(const QList<QUuid>& ids)
	{
		QList<QSharedPointer<Shape>> removedShapes;

		for (const QUuid& id : ids)
		{
			auto it = m_shapesMap.find(id);
			if (it != m_shapesMap.end())
			{
				removedShapes.append(it.value());
				m_shapesMap.erase(it);
			}
		}

		if (!removedShapes.isEmpty())
		{
			emit ShapesRemoved(removedShapes);
		}

		return removedShapes;
	}

	void UpdateShapesGeometry(const QHash<QUuid, QRectF>& rects)
	{
		for (auto posIt = rects.begin(); posIt != rects.end(); ++posIt)
		{
			UpdateShapeGeometry(posIt.key(), posIt.value());
		}
		emit ShapesGeometryChanged(rects);
	}

	void Clear()
	{
		m_shapesMap.clear();
		emit Cleared();
	}

	QHash<QUuid, QRectF> GetShapesGeometry(const QList<QUuid>& ids) const
	{
		QHash<QUuid, QRectF> geometry;
		for (const QUuid& id : ids)
		{
			auto it = m_shapesMap.find(id);
			if (it != m_shapesMap.end())
			{
				geometry.insert(id, it.value()->GetRect());
			}
		}
		return geometry;
	}

	QList<QSharedPointer<Shape>> ListShapes() const
	{
		return m_shapesMap.values();
	}

signals:
	void ShapesAdded(const QList<QSharedPointer<Shape>>& shapes);
	void ShapesRemoved(const QList<QSharedPointer<Shape>>& shapes);
	void ShapesGeometryChanged(const QHash<QUuid, QRectF>& rects);
	void Cleared();

private:
	void UpdateShapeGeometry(const QUuid& id, const QRectF& rect)
	{
		auto it = m_shapesMap.find(id);
		if (it != m_shapesMap.end())
		{
			auto shape = it.value();
			shape->SetGeometry(rect);
			qDebug() << "Updated" << shape->GetId().toString() << "to position and geometry" << rect;
		}
	}

	QHash<QUuid, QSharedPointer<Shape>> m_shapesMap;
};