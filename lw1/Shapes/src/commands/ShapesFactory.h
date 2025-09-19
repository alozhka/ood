#pragma once
#include "../shapes/strategy/IShapeStrategy.h"

#include <string>

class ShapesFactory
{
public:
	static std::unique_ptr<shapes::IShapeStrategy> CreateFromStream(const std::string& type, std::istream& params);
};
