#pragma once
#include "Ellipse.h"
#include "IShapeFactory.h"
#include "Rectangle.h"
#include "RegularPolygon.h"
#include "Triangle.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

class ShapeFactory : public IShapeFactory
{
public:
	std::unique_ptr<Shape> CreateShape(const std::string& description) override
	{
		std::istringstream ss{ description };
		std::string shapeType;

		if (!(ss >> shapeType))
		{
			throw std::runtime_error("Could not read shape description");
		}
		std::ranges::transform(shapeType, shapeType.begin(), [](unsigned char c) { return std::tolower(c); });

		if (shapeType == "regularpolygon")
		{
			return CreateRegularPolygon(ss);
		}
		if (shapeType == "rectangle")
		{
			return CreateRectangle(ss);
		}
		if (shapeType == "triangle")
		{
			return CreateTriangle(ss);
		}
		if (shapeType == "ellipse")
		{
			return CreateEllipse(ss);
		}

		throw std::invalid_argument("Unknown shape type");
	}

private:
	static std::unique_ptr<Shape> CreateRectangle(std::istringstream& ss)
	{
		std::string colorStr;
		double x, y, width, height;

		if (!(ss >> colorStr >> x >> y >> width >> height))
		{
			throw std::invalid_argument("Invalid rectangle format. Expected: rectangle <color> <x> <y> <width> <height>");
		}

		Color color = ParseColor(colorStr);
		return std::make_unique<Rectangle>(color, Point(x, y), width, height);
	}

	static std::unique_ptr<Shape> CreateTriangle(std::istringstream& ss)
	{
		std::string colorStr;
		double x1, y1, x2, y2, x3, y3;

		if (!(ss >> colorStr >> x1 >> y1 >> x2 >> y2 >> x3 >> y3))
		{
			throw std::invalid_argument("Invalid triangle format. Expected: triangle <color> <x1> <y1> <x2> <y2> <x3> <y3>");
		}

		Color color = ParseColor(colorStr);
		return std::make_unique<Triangle>(color, Point(x1, y1), Point(x2, y2), Point(x3, y3));
	}

	static std::unique_ptr<Shape> CreateEllipse(std::istringstream& ss)
	{
		std::string colorStr;
		double centerX, centerY, rx, ry;

		if (!(ss >> colorStr >> centerX >> centerY >> rx >> ry))
		{
			throw std::invalid_argument("Invalid ellipse format. Expected: ellipse <color> <cx> <cx> <rx> <ry>");
		}

		Color color = ParseColor(colorStr);
		return std::make_unique<Ellipse>(color, Point(centerX, centerY), rx, ry);
	}

	static std::unique_ptr<Shape> CreateRegularPolygon(std::istringstream& ss)
	{
		std::string colorStr;
		double centerX, centerY, radius;
		unsigned int vertexCount;

		if (!(ss >> colorStr >> centerX >> centerY >> vertexCount >> radius))
		{
			throw std::invalid_argument("Invalid regular polygon format. Expected: regularpolygon <color> <cx> <cy> <vertexCount> <radius>");
		}

		Color color = ParseColor(colorStr);
		return std::make_unique<RegularPolygon>(color, Point(centerX, centerY), vertexCount, radius);
	}
};