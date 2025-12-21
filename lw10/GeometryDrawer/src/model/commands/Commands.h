#pragma once
#include "../Shape.h"
#include <QHash>
#include <QUndoCommand>

class AddShapeCommand : public QUndoCommand
{
public:
	AddShapeCommand(QHash<QUuid, Shape*>& shapesMap, Shape* shape, QUndoCommand* parent = nullptr)
		: QUndoCommand(parent)
		, m_shapesMap(shapesMap)
		, m_shape(shape)
	{
	}

	void redo() override
	{
		m_shapesMap.insert(m_shape->GetId(), m_shape);
	}

	void undo() override
	{
		m_shapesMap.remove(m_shape->GetId());
	}

private:
	QHash<QUuid, Shape*>& m_shapesMap;
	Shape* m_shape;
};