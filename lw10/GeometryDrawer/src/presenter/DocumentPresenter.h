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

	bool SaveToFile(const QString& filePath);
	bool LoadFromFile(const QString& filePath);

protected:
	bool eventFilter(QObject* object, QEvent* event) override;

private slots:
	void OnShapesAdded(const QList<Shape*>& shape);
	void OnShapesRemoved(const QList<Shape*>& shapes);
	void OnShapesGeometryChanged(const QHash<QUuid, QRectF>& rects);
	void OnDocumentCleared();
	void OnInteractionFinished();

private:
	void AddShape(Shape::Type type);
	void Undo();
	void Redo();
	void MoveShapeWithBounds(ShapeView* shapeView, const QPointF& delta);
	void ResizeShapeWithBounds(ShapeView* shapeView, HandleType type, const QPointF& mousePos);

	ShapeView* ShapeViewFormShape(const Shape* shape);

	static constexpr QRectF DEFAULT_SHAPE_RECT{ 100, 100, 100, 100 };
	static constexpr qreal MIN_SHAPE_SIZE = 20;

	Document* m_document;
	QUndoStack m_history;
	QGraphicsScene* m_scene;
	QMap<QUuid, ShapeView*> m_views;
};