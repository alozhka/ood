#pragma once
#include "../model/Document.h"
#include "../view/EllipseView.h"
#include "../view/RectangleView.h"
#include "../view/ResizeHandle.h"
#include "../view/ShapeView.h"
#include "../view/TriangleView.h"

#include <QGraphicsScene>
#include <QKeyEvent>

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
		m_scene->installEventFilter(this);
	}

	void AddRectangle()
	{
		AddShape(Shape::Type::Rectangle);
	}

	void AddTriangle()
	{
		AddShape(Shape::Type::Triangle);
	}

	void AddEllipse()
	{
		AddShape(Shape::Type::Ellipse);
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

protected:
	bool eventFilter(QObject* object, QEvent* event) override
	{
		if (object == m_scene && event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

			if (keyEvent->key() == Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace)
			{
				RemoveSelectedShapes();
				return true;
			}
		}

		return QObject::eventFilter(object, event);
	}

private slots:
	void OnShapeAdded(Shape* shape)
	{
		ShapeView* shapeView = ShapeViewFormShape(shape);
		connect(shapeView, &ShapeView::InteractionFinished, this, &DocumentPresenter::OnInteractionFinished);
		shapeView->setData(ITEM_ID_KEY, shape->GetId());
		m_scene->addItem(shapeView);
		m_views.insert(shape->GetId(), shapeView);
	}

	void OnShapesRemoved(const QList<QUuid>& ids)
	{
		for (const QUuid& id : ids)
		{
			auto it = m_views.find(id);
			if (it != m_views.end())
			{
				m_scene->removeItem(it.value());
				m_views.erase(it);
			}
		}
	}

	void MoveShapeWithBounds(ShapeView* shapeView, const QPointF& delta)
	{
		QPointF targetPos = shapeView->pos() + delta;

		QRectF sceneRect = m_scene->sceneRect();
		QRectF itemRect = shapeView->boundingRect();

		qreal minX = sceneRect.left() - itemRect.left();
		qreal maxX = sceneRect.right() - itemRect.right();
		qreal minY = sceneRect.top() - itemRect.top();
		qreal maxY = sceneRect.bottom() - itemRect.bottom();

		qreal clampedX = qBound(minX, targetPos.x(), maxX);
		qreal clampedY = qBound(minY, targetPos.y(), maxY);

		shapeView->setPos(clampedX, clampedY);
	}

	void ResizeShapeWithBounds(ShapeView* shapeView, HandleType type, const QPointF& mousePos)
	{
		QPointF sceneMousePos = shapeView->mapToScene(mousePos);
		QRectF currentGeo = shapeView->mapRectToScene(shapeView->GetRect());
		QRectF sceneBounds = m_scene->sceneRect();
		qreal newLeft = currentGeo.left();
		qreal newRight = currentGeo.right();
		qreal newTop = currentGeo.top();
		qreal newBottom = currentGeo.bottom();

		if (type == HandleType::Left || type == HandleType::TopLeft || type == HandleType::BottomLeft)
		{
			qreal minLimit = sceneBounds.left();
			qreal maxLimit = currentGeo.right() - MIN_SHAPE_SIZE;
			newLeft = qBound(minLimit, sceneMousePos.x(), maxLimit);
		}
		else if (type == HandleType::Right || type == HandleType::TopRight || type == HandleType::BottomRight)
		{
			qreal minLimit = currentGeo.left() + MIN_SHAPE_SIZE;
			qreal maxLimit = sceneBounds.right();
			newRight = qBound(minLimit, sceneMousePos.x(), maxLimit);
		}
		if (type == HandleType::Top || type == HandleType::TopLeft || type == HandleType::TopRight)
		{
			qreal minLimit = sceneBounds.top();
			qreal maxLimit = currentGeo.bottom() - MIN_SHAPE_SIZE;

			newTop = qBound(minLimit, sceneMousePos.y(), maxLimit);
		}
		else if (type == HandleType::Bottom || type == HandleType::BottomLeft || type == HandleType::BottomRight)
		{
			qreal minLimit = currentGeo.top() + MIN_SHAPE_SIZE;
			qreal maxLimit = sceneBounds.bottom();
			newBottom = qBound(minLimit, sceneMousePos.y(), maxLimit);
		}

		shapeView->setPos(newLeft, newTop);
		shapeView->SetRect(QRectF(0, 0, newRight - newLeft, newBottom - newTop));
	}

	void OnInteractionFinished()
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

		m_document->UpdateShapesGeometry(shapesGeometry);
	}

private:
	void AddShape(Shape::Type type)
	{
		auto* shape = new Shape(type, DEFAULT_SHAPE_RECT);
		m_document->AddShape(shape);
	}

	ShapeView* ShapeViewFormShape(const Shape* shape)
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
		return shapeView;
	}

	static constexpr int ITEM_ID_KEY = Qt::UserRole + 1;
	static constexpr QRectF DEFAULT_SHAPE_RECT = QRectF(100, 100, 100, 100);
	static constexpr qreal MIN_SHAPE_SIZE = 20;

	Document* m_document;
	QGraphicsScene* m_scene;
	QMap<QUuid, ShapeView*> m_views;
};