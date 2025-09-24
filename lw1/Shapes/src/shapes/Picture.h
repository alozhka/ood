#pragma once
#include "Shape.h"

#include <unordered_map>
#include <vector>

namespace shapes
{
class Picture
{
public:
	Picture() = default;
	Picture(const Picture& other);

	void AddShape(std::unique_ptr<Shape>&& shape);
	void RemoveShape(const std::string& id);

	void MoveShape(int id, double x, double y);
	void MovePicture(double x, double y);

	void DrawShape(const std::string& id, gfx::ICanvas& canvas);
	void DrawPicture(gfx::ICanvas& canvas);

	std::vector<Shape*> ListShapes() const;

private:
	Shape* GetShape(const std::string& id) const;

	std::unordered_map<std::string, std::unique_ptr<Shape>> m_shapes{};
};
} // namespace shapes
