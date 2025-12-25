#pragma once
#include "../model/Document.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

class DocumentRepository
{
public:
	static bool SaveToFile(const Document* document, const QString& filePath)
	{
		QJsonArray shapesArray;

		QList<Shape*> shapes = document->GetAllShapes();
		for (const Shape* shape : shapes)
		{
			QJsonObject shapeObj;
			shapeObj["type"] = static_cast<int>(shape->GetType());

			QRectF rect = shape->GetRect();
			shapeObj["x"] = rect.x();
			shapeObj["y"] = rect.y();
			shapeObj["width"] = rect.width();
			shapeObj["height"] = rect.height();

			shapesArray.append(shapeObj);
		}

		QJsonObject rootObj;
		rootObj["version"] = 1;
		rootObj["shapes"] = shapesArray;

		QJsonDocument doc(rootObj);

		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			qWarning() << "Failed to open file for writing:" << filePath;
			return false;
		}

		file.write(doc.toJson());
		file.close();

		return true;
	}

	static QList<Shape*> LoadFromFile(const QString& filePath)
	{
		QList<Shape*> shapes;

		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
		{
			qWarning() << "Failed to open file for reading:" << filePath;
			return shapes;
		}

		QByteArray data = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(data);
		if (!doc.isObject())
		{
			qWarning() << "Invalid JSON format";
			return shapes;
		}

		QJsonObject rootObj = doc.object();
		if (!rootObj.contains("shapes") || !rootObj["shapes"].isArray())
		{
			qWarning() << "JSON does not contain shapes array";
			return shapes;
		}

		QJsonArray shapesArray = rootObj["shapes"].toArray();
		for (const QJsonValue& value : shapesArray)
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

			Shape::Type type = static_cast<Shape::Type>(typeInt);
			QRectF rect(x, y, width, height);

			Shape* shape = new Shape(type, rect);
			shapes.append(shape);
		}

		return shapes;
	}
};