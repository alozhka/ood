#pragma once
#include "../model/Shape.h"
#include <QHash>
#include <QUndoCommand>

class AddShapeCommand : public QUndoCommand
{
public:
	AddShapeCommand(Document* document, Shape* shape, QUndoCommand* parent = nullptr)
		: QUndoCommand(parent)
		, m_document(document)
		, m_shape(shape)
	{
		setText("Add Shape");
	}

	void redo() override
	{
		m_document->AddShapes({ m_shape });
	}

	void undo() override
	{
		m_document->RemoveShapes({ m_shape->GetId() });
	}

private:
	Document* m_document;
	Shape* m_shape;
};

class RemoveShapesCommand : public QUndoCommand
{
public:
	RemoveShapesCommand(Document* document, const QList<QUuid>& shapeIdsToRemove, QUndoCommand* parent = nullptr)
		: QUndoCommand(parent)
		, m_document(document)
		, m_shapeIdsToRemove(shapeIdsToRemove)
	{
	}

	void redo() override
	{
		m_removedShapes = m_document->RemoveShapes(m_shapeIdsToRemove);
	}

	void undo() override
	{
		m_document->AddShapes(m_removedShapes);
	}

private:
	Document* m_document;
	QList<Shape*> m_removedShapes{};
	QList<QUuid> m_shapeIdsToRemove;
};