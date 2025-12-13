#pragma once
#include <QGraphicsView>

class CanvasPresenter : public QObject
{
public:
	explicit CanvasPresenter(QGraphicsScene* scene, QObject* parent = nullptr);

	void AddRectangle();
	void AddTriangle();
	void AddEllipse();

private:
	QGraphicsScene* m_scene;
	static constexpr uint DEFAULT_SHAPE_SIZE = 100;
};
