#include "../commands/ShapesFactory.h"
#include "../shapes/strategy/ShapesStrategies.h"

#include <istream>

namespace
{
std::unique_ptr<shapes::CircleStrategy> CreateCircleFromStream(std::istream& params)
{
	double x, y, radius;
	if (!(params >> x >> y >> radius))
	{
		throw std::runtime_error("Invalid circle parameters");
	}
	return std::make_unique<shapes::CircleStrategy>(x, y, radius);
}

std::unique_ptr<shapes::RectangleStrategy> CreateRectangleFromStream(std::istream& params)
{
	double left, top, width, height;
	if (!(params >> left >> top >> width >> height))
	{
		throw std::runtime_error("Invalid rectangle parameters");
	}
	return std::make_unique<shapes::RectangleStrategy>(left, top, width, height);
}

std::unique_ptr<shapes::TriangleStrategy> CreateTriangleFromStream(std::istream& params)
{
	double x1, y1, x2, y2, x3, y3;
	if (!(params >> x1 >> y1 >> x2 >> y2 >> x3 >> y3))
	{
		throw std::runtime_error("Invalid triangle parameters");
	}
	return std::make_unique<shapes::TriangleStrategy>(x1, y1, x2, y2, x3, y3);
}

std::unique_ptr<shapes::LineStrategy> CreateLineFromStream(std::istream& params)
{
	double x1, y1, x2, y2;
	if (!(params >> x1 >> y1 >> x2 >> y2))
	{
		throw std::runtime_error("Invalid line parameters");
	}
	return std::make_unique<shapes::LineStrategy>(x1, y1, x2, y2);
}

std::unique_ptr<shapes::TextStrategy> CreateTextFromStream(std::istream& params)
{
	double left, top, size;
	std::string text;
	if (!(params >> left >> top >> size) || !std::getline(params, text))
	{
		throw std::runtime_error("Invalid text parameters");
	}
	return std::make_unique<shapes::TextStrategy>(left, top, size, text);
}
} // namespace

std::unique_ptr<shapes::IShapeStrategy> ShapesFactory::CreateFromStream(const std::string& type, std::istream& params)
{
	if (type == "circle")
	{
		return CreateCircleFromStream(params);
	}
	if (type == "rectangle")
	{
		return CreateRectangleFromStream(params);
	}
	if (type == "triangle")
	{
		return CreateTriangleFromStream(params);
	}
	if (type == "line")
	{
		return CreateLineFromStream(params);
	}
	if (type == "text")
	{
		return CreateTextFromStream(params);
	}

	throw std::invalid_argument("Unknown shape type");
}