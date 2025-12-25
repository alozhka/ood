#pragma once
#include "../model/Document.h"
#include "../view/ShapeView.h"

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
	void AddImage(const QString& sourceImagePath);
	void RemoveSelectedShapes();

	void SaveToFile(const QString& filePath);
	void LoadFromFile(const QString& filePath);

protected:
	bool eventFilter(QObject* object, QEvent* event) override;

private slots:
	void OnShapesAdded(const QList<QSharedPointer<Shape>>& shape);
	void OnShapesRemoved(const QList<QSharedPointer<Shape>>& shapes);
	void OnShapesGeometryChanged(const QHash<QUuid, QRectF>& rects);
	void OnDocumentCleared();
	void OnInteractionFinished();

private:
	void AddShape(Shape::Type type);
	void Undo();
	void Redo();
	void MoveShapeWithBounds(const QPointF& delta);
	void ResizeShapeWithBounds(ShapeView* shapeView, HandleType type, const QPointF& mousePos);

	ShapeView* ShapeViewFormShape(const Shape* shape);
	int GetNextLayer() const;

	static constexpr QRectF DEFAULT_SHAPE_RECT{ 100, 100, 100, 100 };
	static constexpr qreal MIN_SHAPE_SIZE = 20;

	Document* m_document;
	QUndoStack m_history;
	QGraphicsScene* m_scene;
	QMap<QUuid, ShapeView*> m_views;
};