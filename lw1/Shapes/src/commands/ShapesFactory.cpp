
#include "../commands/ShapesFactory.h"
std::unique_ptr<shapes::IShapeStrategy> ShapesFactory::CreateFromStream(const std::string& type, std::istream& params)
{
	if (type == "circle")
	{
		return std::make_unique<>()
	}
}