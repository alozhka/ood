#pragma once
#include "../shapes/IShapeStrategy.h"

#include <string>

class ShapesFactory
{
public:
	static IShapeStrategy CreateFromStream(const std::string& type, std::istream& params);
};
