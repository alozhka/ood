#pragma once
#include "../Shape.h"
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
	}

	void redo() override
	{
		m_document->AddShape(m_shape);
	}

	void undo() override
	{
		m_document->RemoveShapes({ m_shape->GetId() });
	}

private:
	Document* m_document;
	Shape* m_shape;
};