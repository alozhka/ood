#include "../commands/ShapesFactory.h"
#include "../shapes/strategy/ShapesStrategies.h"

#include <istream>

std::unique_ptr<shapes::IShapeStrategy> ShapesFactory::CreateFromStream(const std::string& type, std::istream& params)
{
	if (type == "circle")
	{
		double x, y, radius;
		if (!(params >> x >> y >> radius))
		{
			throw std::runtime_error("Invalid circle parameters");
		}
		return std::make_unique<shapes::CircleStrategy>(x, y, radius);
	}

	if (type == "rectangle")
	{
		double left, top, width, height;
		if (!(params >> left >> top >> width >> height))
		{
			throw std::runtime_error("Invalid rectangle parameters");
		}
		return std::make_unique<shapes::RectangleStrategy>(left, top, width, height);
	}

	if (type == "triangle")
	{
		double x0, y0, x1, y1, x2, y2;
		if (!(params >> x0 >> y0 >> x1 >> y1 >> x2 >> y2))
		{
			throw std::runtime_error("Invalid triangle parameters");
		}
	}

	if (type == "line")
	{
		double x1, y1, x2, y2;
		if (!(params >> x1 >> y1 >> x2 >> y2))
		{
			throw std::runtime_error("Invalid line parameters");
		}
		return std::make_unique<shapes::LineStrategy>(x1, y1, x2, y2);
	}

	if (type == "text")
	{
		double left, top, size;
		std::string text;
		if (!(params >> left >> top >> size) || !std::getline(params, text))
		{
			throw std::runtime_error("Invalid text parameters");
		}
		return std::make_unique<shapes::TextStrategy>(left, top, size, text);
	}

	throw std::invalid_argument("Unknown shape type");
}