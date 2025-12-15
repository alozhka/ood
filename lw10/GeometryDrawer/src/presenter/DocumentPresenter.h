#pragma once
#include "../model/Document.h"
#include "../view/EllipseView.h"
#include "../view/RectangleView.h"
#include "../view/ShapeView.h"
#include "../view/TriangleView.h"

#include <QGraphicsScene>

class DocumentPresenter : public QObject
{
	Q_OBJECT
public:
	DocumentPresenter(Document* document, QGraphicsScene* scene, QObject* parent = nullptr)
		: QObject(parent)
		, m_document(document)
		, m_scene(scene)
	{
		connect(m_document, &Document::ShapeAdded, this, &DocumentPresenter::OnShapeAdded);
		connect(m_document, &Document::ShapesRemoved, this, &DocumentPresenter::OnShapesRemoved);
	}

	void AddShape(Shape::Type type)
	{
		auto* shape = new Shape(type, QRectF(0, 0, 100, 100));
		m_document->AddShape(shape);
	}

	void RemoveSelectedShapes()
	{
		QList<QUuid> shapeIdsToRemove;
		for (QGraphicsItem* item : m_scene->selectedItems())
		{
			QVariant data = item->data(ITEM_ID_KEY);

			if (!data.isValid())
			{
				continue;
			}

			shapeIdsToRemove.append(data.toUuid());
		}

		m_document->RemoveShapes(shapeIdsToRemove);
	}

private slots:
	void OnShapeAdded(Shape* shape)
	{
		ShapeView* shapeView = nullptr;
		switch (shape->GetType())
		{
		case Shape::Type::Rectangle:
			shapeView = new RectangleView(shape->GetRect());
			break;
		case Shape::Type::Triangle:
			shapeView = new TriangleView(shape->GetRect());
			break;
		case Shape::Type::Ellipse:
			shapeView = new EllipseView(shape->GetRect());
			break;
		default:
			throw std::runtime_error("Unknown shape type");
		}

		shapeView->setData(ITEM_ID_KEY, shape->GetId());
		m_scene->addItem(shapeView);
		m_views.insert(shape->GetId(), shapeView);
	}

	void OnShapesRemoved(const QList<QUuid>& ids)
	{
		for (const QUuid& id : ids)
		{
			auto shapeView = m_views.take(id);
			m_scene->removeItem(shapeView);
			delete shapeView;
		}
	}

private:
	static constexpr int ITEM_ID_KEY = 0;

	Document* m_document;
	QGraphicsScene* m_scene;
	QMap<QUuid, ShapeView*> m_views;
};