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
	}

	void AddShape(Shape::Type type)
	{
		auto* shape = new Shape(type, QRectF(0, 0, 100, 100));
		m_document->AddShape(shape);
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

		m_scene->addItem(shapeView);
		m_views.insert(shape, shapeView);
	}

private:
	Document* m_document;
	QGraphicsScene* m_scene;
	QMap<Shape*, ShapeView*> m_views;
};