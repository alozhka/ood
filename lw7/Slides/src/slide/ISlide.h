#pragma once
#include "../shape/Shape.h"
#include <memory>
#include <set>

class ISlide
{
public:
	virtual void AddShape(const std::shared_ptr<IShape>& shape) = 0;
	virtual void TransformShape(int index, double left, double top, double width, double height) = 0;
	virtual void SetLineStyle(int index, RGBAColor color) = 0;
	virtual void SetFillStyle(int index, RGBAColor color) = 0;

	virtual void GroupShapes(const std::set<int>& indexes) = 0;

	virtual size_t GetShapesCount() const = 0;
	virtual std::shared_ptr<IShape> GetShapeAt(size_t index) = 0;

	virtual ~ISlide() = default;
};
