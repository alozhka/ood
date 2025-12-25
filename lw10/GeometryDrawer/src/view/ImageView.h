#pragma once
#include "ShapeView.h"

#include <QPixmap>
#include <QString>

class ImageView final : public ShapeView
{
	Q_OBJECT
public:
	explicit ImageView(
		const QRectF& rect,
		const QString& imagePath,
		const MovementHandler& movementHandler,
		const ResizeHandler& resizeHandler,
		QGraphicsItem* parent = nullptr)
		: ShapeView(rect, movementHandler, resizeHandler, parent)
		, m_imagePath(imagePath)
		, m_loadAttempted(false)
	{
	}

	QString GetImagePath() const
	{
		return m_imagePath;
	}

protected:
	void PaintShape(QPainter* painter) override
	{
		if (!m_loadAttempted)
		{
			LoadImage();
			m_loadAttempted = true;
		}

		if (!m_pixmap.isNull())
		{
			painter->drawPixmap(m_rect.toRect(), m_pixmap);
		}
		else
		{
			painter->setBrush(QColor(220, 220, 220));
			painter->setPen(QPen(Qt::darkGray, 2, Qt::DashLine));
			painter->drawRect(m_rect);

			painter->setPen(Qt::darkGray);
			QFont font = painter->font();
			font.setPixelSize(14);
			painter->setFont(font);
			painter->drawText(m_rect, Qt::AlignCenter, "Image\nnot found");
		}
	}

private:
	void LoadImage()
	{
		if (m_imagePath.isEmpty())
		{
			return;
		}

		if (!m_pixmap.load(m_imagePath))
		{
			qWarning() << "Failed to load image:" << m_imagePath;
		}
	}

	QString m_imagePath;
	QPixmap m_pixmap;
	bool m_loadAttempted;
};