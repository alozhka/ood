#include "CanvasPresenter.h"
#include <QGraphicsRectItem>

CanvasPresenter::CanvasPresenter(QGraphicsScene* scene, QObject* parent)
	: QObject(parent)
	, m_scene(scene)
{
}

void CanvasPresenter::AddRectangle()
{
	QRectF sceneRect = m_scene->sceneRect();

	double x = sceneRect.center().x() - DEFAULT_SHAPE_SIZE / 2;
	double y = sceneRect.center().y() - DEFAULT_SHAPE_SIZE / 2;

	QGraphicsRectItem* rect = m_scene->addRect(
		x,
		y,
		DEFAULT_SHAPE_SIZE,
		DEFAULT_SHAPE_SIZE,
		QPen(Qt::black),
		QBrush(Qt::lightGray));

	rect->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	rect->setData(0, "rectangle");
}

void CanvasPresenter::AddTriangle()
{
	QRectF sceneRect = m_scene->sceneRect();

	double cx = sceneRect.center().x() - DEFAULT_SHAPE_SIZE / 2;
	double cy = sceneRect.center().y() - DEFAULT_SHAPE_SIZE / 2;
	double r = DEFAULT_SHAPE_SIZE / 2;

	QPolygonF triangle;
	triangle << QPointF(cx, cy - r) << QPointF(cx - r, cy + r) << QPoint(cx + r, cy + r);
	QGraphicsPolygonItem* polygon = m_scene->addPolygon(triangle, QPen(Qt::black), QBrush(Qt::lightGray));
	polygon->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	polygon->setData(0, "triangle");
}

void CanvasPresenter::AddEllipse()
{
	QRectF sceneRect = m_scene->sceneRect();

	double cx = sceneRect.center().x();
	double cy = sceneRect.center().y();

	auto ellipse = m_scene->addEllipse(
		cx - DEFAULT_SHAPE_SIZE / 2,
		cy - DEFAULT_SHAPE_SIZE / 2,
		DEFAULT_SHAPE_SIZE,
		DEFAULT_SHAPE_SIZE,
		QPen(Qt::black),
		QBrush(Qt::lightGray));
	ellipse->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	ellipse->setData(0, "ellipse");
}