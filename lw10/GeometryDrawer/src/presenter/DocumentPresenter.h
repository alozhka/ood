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
	DocumentPresenter(Document* document, QGraphicsScene* scene, QObject* parent = nullptr);

	void AddRectangle();
	void AddTriangle();
	void AddEllipse();
	void RemoveSelectedShapes();

protected:
	bool eventFilter(QObject* object, QEvent* event) override;

private slots:
	void OnShapeAdded(Shape* shape);
	void OnShapesRemoved(const QList<QUuid>& ids);
	void MoveShapeWithBounds(ShapeView* shapeView, const QPointF& delta);
	void ResizeShapeWithBounds(ShapeView* shapeView, HandleType type, const QPointF& mousePos);
	void OnInteractionFinished();

private:
	void AddShape(Shape::Type type);
	void Undo();
	void Redo();
	ShapeView* ShapeViewFormShape(const Shape* shape);

	static constexpr int ITEM_ID_KEY = Qt::UserRole + 1;
	static constexpr QRectF DEFAULT_SHAPE_RECT{ 100, 100, 100, 100 };
	static constexpr qreal MIN_SHAPE_SIZE = 20;

	Document* m_document;
	QUndoStack m_history;
	QGraphicsScene* m_scene;
	QMap<QUuid, ShapeView*> m_views;
};