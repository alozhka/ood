#pragma once
#include "../model/Document.h"
#include "../view/EllipseView.h"
#include "../view/RectangleView.h"
#include "../view/ResizeHandle.h"
#include "../view/ShapeView.h"
#include "../view/TriangleView.h"

#include <QGraphicsScene>
#include <QToolButton>

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
		connect(shapeView, &ShapeView::MovementFinished, this, &DocumentPresenter::OnMovementFinished);
		shapeView->SetMovementHandler([this](ShapeView* shapeView, const QPointF& delta) {
			MoveShapeWithBounds(shapeView, delta);
		});
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

	void OnMovementFinished()
	{
		QHash<QUuid, QPointF> shapesPositions;

		for (QGraphicsItem* item : m_scene->selectedItems())
		{
			QVariant data = item->data(ITEM_ID_KEY);

			if (!data.isValid())
			{
				continue;
			}

			shapesPositions.insert(data.toUuid(), item->pos());
		}

		m_document->UpdateShapePositions(shapesPositions);
	}

	void ResizeShapeWithBounds(ShapeView* shapeView, HandleType type, const QPointF& mousePos)
	{
		// 1. Текущие абсолютные координаты фигуры
		QRectF currentRect = shapeView->mapRectToScene(shapeView->boundingRect());

		// 2. Копии для модификации
		qreal left = currentRect.left();
		qreal right = currentRect.right();
		qreal top = currentRect.top();
		qreal bottom = currentRect.bottom();

		// 3. Изменяем стороны в зависимости от того, какую ручку тянем
		// Тут мы подменяем одну из координат на mousePos

		switch (type)
		{
		case HandleType::Left:
			left = mousePos.x();
			break;
		case HandleType::Right:
			right = mousePos.x();
			break;
		case HandleType::Top:
			top = mousePos.y();
			break;
		case HandleType::Bottom:
			bottom = mousePos.y();
			break;
		case HandleType::TopLeft:
			left = mousePos.x();
			top = mousePos.y();
			break;
		case HandleType::TopRight:
			right = mousePos.x();
			top = mousePos.y();
			break;
		case HandleType::BottomLeft:
			left = mousePos.x();
			bottom = mousePos.y();
			break;
		case HandleType::BottomRight:
			right = mousePos.x();
			bottom = mousePos.y();
			break;
		}

		// 4. Нормализация (чтобы left не стал больше right)
		if (left > right)
			std::swap(left, right);
		if (top > bottom)
			std::swap(top, bottom);

		// 5. Проверка минимального размера (например 10px)
		if (right - left < 10)
			return;
		if (bottom - top < 10)
			return;

		// 6. Проверка границ сцены (можно переиспользовать qBound как в movement)
		QRectF sceneRect = m_scene->sceneRect();
		// ... код проверки, чтобы left >= sceneRect.left() и т.д. ...

		// 7. Применение изменений
		// Важно: Resizing меняет И позицию (pos), И размер (rect)
		// Самый простой способ обновить View:
		QRectF newRect(left, top, right - left, bottom - top);

		// setPos ставит левый верхний угол
		shapeView->setPos(newRect.topLeft());
		// setRect задает внутренние размеры (от 0,0 до w,h)
		shapeView->SetRect(QRectF(0, 0, newRect.width(), newRect.height()));
	}

private:
	void AddShape(Shape::Type type)
	{
		auto* shape = new Shape(type, DEFAULT_SHAPE_RECT);
		m_document->AddShape(shape);
	}

	static constexpr int ITEM_ID_KEY = Qt::UserRole + 1;
	static constexpr QRectF DEFAULT_SHAPE_RECT = QRectF(100, 100, 100, 100);

	Document* m_document;
	QGraphicsScene* m_scene;
	QMap<QUuid, ShapeView*> m_views;
};