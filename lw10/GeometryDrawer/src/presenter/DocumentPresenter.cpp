#include "DocumentPresenter.h"

#include "Commands.h"
#include "ShapeManipulator.h"

namespace
{
constexpr int ITEM_ID_KEY = Qt::UserRole + 1;
}

DocumentPresenter::DocumentPresenter(Document* document, QGraphicsScene* scene, QObject* parent)
	: QObject(parent)
	, m_document(document)
	, m_history(this)
	, m_scene(scene)
{
	connect(m_document, &Document::ShapesAdded, this, &DocumentPresenter::OnShapesAdded);
	connect(m_document, &Document::ShapesRemoved, this, &DocumentPresenter::OnShapesRemoved);
	connect(m_document, &Document::ShapesGeometryChanged, this, &DocumentPresenter::OnShapesGeometryChanged);
	m_scene->installEventFilter(this);
}

void DocumentPresenter::AddRectangle()
{
	AddShape(Shape::Type::Rectangle);
}

void DocumentPresenter::AddTriangle()
{
	AddShape(Shape::Type::Triangle);
}

void DocumentPresenter::AddEllipse()
{
	AddShape(Shape::Type::Ellipse);
}

void DocumentPresenter::RemoveSelectedShapes()
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

	RemoveShapesCommand* command = new RemoveShapesCommand(m_document, shapeIdsToRemove);
	m_history.push(command);
}

bool DocumentPresenter::eventFilter(QObject* object, QEvent* event)
{
	if (object == m_scene && event->type() == QEvent::KeyPress)
	{
		QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);

		if (keyEvent->matches(QKeySequence::Backspace) || keyEvent->matches(QKeySequence::Delete))
		{
			RemoveSelectedShapes();
			return true;
		}

		if (keyEvent->matches(QKeySequence::Undo))
		{
			Undo();
			return true;
		}

		if (keyEvent->matches(QKeySequence::Redo))
		{
			Redo();
			return true;
		}
	}

	return QObject::eventFilter(object, event);
}

void DocumentPresenter::OnShapesAdded(const QList<Shape*>& shapes)
{
	m_scene->clearSelection();
	for (Shape* shape : shapes)
	{
		ShapeView* shapeView = ShapeViewFormShape(shape);
		connect(shapeView, &ShapeView::InteractionFinished, this, &DocumentPresenter::OnInteractionFinished);
		shapeView->setData(ITEM_ID_KEY, shape->GetId());
		m_scene->addItem(shapeView);
		m_views.insert(shape->GetId(), shapeView);
		shapeView->setSelected(true);
	}
}

void DocumentPresenter::OnShapesRemoved(const QList<Shape*>& shapes)
{
	for (const Shape* shape : shapes)
	{
		auto it = m_views.find(shape->GetId());
		if (it != m_views.end())
		{
			m_scene->removeItem(it.value());
			m_views.erase(it);
		}
	}
}

void DocumentPresenter::OnShapesGeometryChanged(const QHash<QUuid, QRectF>& geometry)
{
	for (auto it = geometry.begin(); it != geometry.end(); ++it)
	{
		const QUuid& id = it.key();
		const QRectF& newRect = it.value();

		auto viewIt = m_views.find(id);
		if (viewIt != m_views.end())
		{
			ShapeView* shapeView = viewIt.value();
			shapeView->setPos(newRect.topLeft());
			shapeView->SetRect(QRectF(0, 0, newRect.width(), newRect.height()));
			shapeView->setSelected(true);
		}
	}
}

void DocumentPresenter::MoveShapeWithBounds(ShapeView* shapeView, const QPointF& delta)
{
	ShapeManipulator::MoveShapeWithBounds(shapeView, delta, m_scene->sceneRect());
}

void DocumentPresenter::ResizeShapeWithBounds(ShapeView* shapeView, HandleType type, const QPointF& mousePos)
{
	ShapeManipulator::ResizeShapeWithBounds(shapeView, type, mousePos, m_scene->sceneRect(), MIN_SHAPE_SIZE);
}

void DocumentPresenter::OnInteractionFinished()
{
	QHash<QUuid, QRectF> shapesGeometry;

	for (QGraphicsItem* item : m_scene->selectedItems())
	{
		QVariant data = item->data(ITEM_ID_KEY);

		if (!data.isValid())
		{
			continue;
		}

		shapesGeometry.insert(data.toUuid(), item->mapRectToScene(item->boundingRect()));
	}

	if (shapesGeometry.isEmpty())
	{
		return;
	}

	QHash<QUuid, QRectF> oldGeometry = m_document->GetShapesGeometry(shapesGeometry.keys());
	UpdateShapesGeometryCommand* command = new UpdateShapesGeometryCommand(m_document, oldGeometry, shapesGeometry);
	m_history.push(command);
}

void DocumentPresenter::AddShape(Shape::Type type)
{
	Shape* shape = new Shape(type, DEFAULT_SHAPE_RECT);
	AddShapeCommand* command = new AddShapeCommand(m_document, shape);
	m_history.push(command);
}

void DocumentPresenter::Undo()
{
	if (m_history.canUndo())
	{
		m_history.undo();
	}
}

void DocumentPresenter::Redo()
{
	if (m_history.canRedo())
	{
		m_history.redo();
	}
}

ShapeView* DocumentPresenter::ShapeViewFormShape(const Shape* shape)
{
	ShapeView* shapeView = nullptr;
	switch (shape->GetType())
	{
	case Shape::Type::Rectangle:
		shapeView = new RectangleView(
			shape->GetRect(),
			std::bind_front(&DocumentPresenter::MoveShapeWithBounds, this),
			std::bind_front(&DocumentPresenter::ResizeShapeWithBounds, this));
		break;
	case Shape::Type::Triangle:
		shapeView = new TriangleView(
			shape->GetRect(),
			std::bind_front(&DocumentPresenter::MoveShapeWithBounds, this),
			std::bind_front(&DocumentPresenter::ResizeShapeWithBounds, this));
		break;
	case Shape::Type::Ellipse:
		shapeView = new EllipseView(
			shape->GetRect(),
			std::bind_front(&DocumentPresenter::MoveShapeWithBounds, this),
			std::bind_front(&DocumentPresenter::ResizeShapeWithBounds, this));
		break;
	default:
		throw std::runtime_error("Unknown shape type");
	}
	shapeView->setParent(this);
	return shapeView;
}