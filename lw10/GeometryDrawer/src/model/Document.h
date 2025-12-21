#pragma once
#include "Shape.h"
#include "commands/Commands.h"

#include <QDebug>
#include <QHash>
#include <QObject>
#include <QUndoStack>

class Document : public QObject
{
	Q_OBJECT
public:
	explicit Document(QObject* parent = nullptr)
		: QObject(parent)
	{
	}

	void AddShape(Shape::Type type)
	{
		Shape* shape = new Shape(type, DEFAULT_SHAPE_RECT, this);
		AddShapeCommand* command = new AddShapeCommand(m_shapesMap, shape);
		m_history.push(command);
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

	void UpdateShapesGeometry(const QHash<QUuid, QRectF>& rects)
	{
		for (auto posIt = rects.begin(); posIt != rects.end(); ++posIt)
		{
			UpdateShapeGeometry(posIt.key(), posIt.value());
		}
	}

	void Undo()
	{
		m_history.undo();
	}

	void Redo()
	{
		m_history.redo();
	}

	bool CanUndo() const
	{
		return m_history.canUndo();
	}

	bool CanRedo() const
	{
		return m_history.canRedo();
	}

signals:
	void ShapeAdded(Shape* shape);
	void ShapesRemoved(const QList<QUuid>& ids);

private:
	void UpdateShapeGeometry(const QUuid& id, const QRectF& rect)
	{
		auto it = m_shapesMap.find(id);
		if (it != m_shapesMap.end())
		{
			Shape* shape = it.value();
			shape->SetGeometry(rect);
			qDebug() << "Updated " << shape->GetId().toString() << " to position and geometry " << rect;
		}
	}

	static constexpr QRectF DEFAULT_SHAPE_RECT{ 100, 100, 100, 100 };

	QHash<QUuid, Shape*> m_shapesMap;
	QUndoStack m_history;
};