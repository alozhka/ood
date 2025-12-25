#pragma once
#include "../model/Document.h"
#include "ImageRepository.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <stdexcept>

class JsonDocumentRepository
{
public:
	static void SaveToFile(const Document* document, const QString& filePath)
	{
		QJsonArray shapesArray = ShapesToJson(document->GetAllShapes(), filePath);
		QJsonObject rootObj;
		rootObj["shapes"] = shapesArray;
		QJsonDocument doc(rootObj);

		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			throw std::runtime_error("Failed to open file");
		}

		file.write(doc.toJson());
		file.close();
	}

	static QList<Shape*> LoadFromFile(const QString& filePath)
	{
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
		{
			throw std::runtime_error("Failed to open file");
		}

		QByteArray data = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(data);
		if (!doc.isObject())
		{
			throw std::runtime_error("Invalid file format");
		}

		QJsonObject rootObj = doc.object();
		EnsureValidRootObject(rootObj);

		QJsonArray shapesArray = rootObj["shapes"].toArray();
		return JsonToShapes(shapesArray, filePath);
	}

private:
	static QJsonArray ShapesToJson(const QList<Shape*>& shapes, const QString& filePath)
	{
        QFileInfo fileInfo(filePath);
		QString documentDir = fileInfo.absolutePath();
		QString baseName = fileInfo.completeBaseName();
		QString imagesDir = documentDir + "/" + baseName + "_images";

		QJsonArray shapesArray;

		for (const Shape* shape : shapes)
		{
			QJsonObject shapeObj;
			shapeObj["type"] = static_cast<int>(shape->GetType());

			QRectF rect = shape->GetRect();
			shapeObj["x"] = rect.x();
			shapeObj["y"] = rect.y();
			shapeObj["width"] = rect.width();
			shapeObj["height"] = rect.height();
			shapeObj["layer"] = shape->GetLayer();

			if (shape->GetType() == Shape::Type::Image && !shape->GetImagePath().isEmpty())
			{
				QString fileName = ImageRepository::Export(
					shape->GetImagePath(),
					imagesDir);

				QString relativePath = baseName + "_images/" + fileName;
				shapeObj["imagePath"] = relativePath;
			}

			shapesArray.append(shapeObj);
		}

		return shapesArray;
	}

	static void EnsureValidRootObject(const QJsonObject& rootObj)
	{
		if (!rootObj.contains("shapes") || !rootObj["shapes"].isArray())
		{
			throw std::runtime_error("Failed to parse JSON");
		}
	}

	static QList<Shape*> JsonToShapes(const QJsonArray& shapesArray, const QString& filePath)
	{
		QFileInfo fileInfo(filePath);
		QString documentDir = fileInfo.absolutePath();

		QList<Shape*> shapes;

		for (const QJsonValueConstRef& value : shapesArray)
		{
			if (!value.isObject())
			{
				continue;
			}

			QJsonObject shapeObj = value.toObject();

			int typeInt = shapeObj["type"].toInt();
			qreal x = shapeObj["x"].toDouble();
			qreal y = shapeObj["y"].toDouble();
			qreal width = shapeObj["width"].toDouble();
			qreal height = shapeObj["height"].toDouble();
			int layer = shapeObj["layer"].toInt(0);

			Shape::Type type = static_cast<Shape::Type>(typeInt);
			QRectF rect(x, y, width, height);

			Shape* shape = new Shape(type, rect, layer);

			if (type == Shape::Type::Image && shapeObj.contains("imagePath"))
			{
				QString relativePath = shapeObj["imagePath"].toString();
				QString tempPath = ImageRepository::LoadFromDocument(
					relativePath,
					documentDir);
				shape->SetImagePath(tempPath);
			}

			shapes.append(shape);
		}

		return shapes;
	}
};