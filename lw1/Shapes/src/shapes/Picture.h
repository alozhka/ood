#pragma once
#include "Shape.h"

#include <unordered_map>

namespace shapes
{
class Picture
{
public:
	Picture();
	Picture(const Picture& other);

	void AddShape(std::unique_ptr<Shape> shape);
	void RemoveShape(int id);
	std::weak_ptr<Shape> GetShape(int id) const;

	void MoveShape(int id, double x, double y);
	void MovePicture(double x, double y);

private:
	std::unordered_map<std::string, std::unique_ptr<Shape>> m_shapes;
};
} // namespace Shapes
