#pragma once
#include "../model/Document.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <stdexcept>

class DocumentRepository
{
public:
	static void SaveToFile(const Document* document, const QString& filePath)
	{
		QJsonArray shapesArray = ShapesToJson(document->GetAllShapes());
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
		return JsonToShapes(shapesArray);
	}

private:
	static QJsonArray ShapesToJson(const QList<Shape*>& shapes)
	{
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

	static QList<Shape*> JsonToShapes(const QJsonArray& shapesArray)
	{
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
			shapes.append(shape);
		}

		return shapes;
	}
};