#pragma once
#include "../model/ImageStorage.h"
#include "../model/Shape.h"
#include <QHash>
#include <QSharedPointer>
#include <QUndoCommand>

class AddShapeCommand : public QUndoCommand
{
public:
	AddShapeCommand(Document* document, const QSharedPointer<Shape>& shape, QUndoCommand* parent = nullptr)
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
	QSharedPointer<Shape> m_shape;
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
	QList<QSharedPointer<Shape>> m_removedShapes{};
	QList<QUuid> m_shapeIdsToRemove;
};

class UpdateShapesGeometryCommand : public QUndoCommand
{
public:
	UpdateShapesGeometryCommand(Document* document, const QHash<QUuid, QRectF>& oldGeometry, const QHash<QUuid, QRectF>& newGeometry, QUndoCommand* parent = nullptr)
		: QUndoCommand(parent)
		, m_document(document)
		, m_oldGeometry(oldGeometry)
		, m_newGeometry(newGeometry)
	{
		setText("Update Shape Geometry");
	}

	void redo() override
	{
		m_document->UpdateShapesGeometry(m_newGeometry);
	}

	void undo() override
	{
		m_document->UpdateShapesGeometry(m_oldGeometry);
	}

private:
	Document* m_document;
	QHash<QUuid, QRectF> m_oldGeometry;
	QHash<QUuid, QRectF> m_newGeometry;
};