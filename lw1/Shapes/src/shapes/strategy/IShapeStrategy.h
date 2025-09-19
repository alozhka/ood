#pragma once

#include "../../draw/ICanvas.h"

namespace shapes
{
class IShapeStrategy
{
public:
	virtual void Draw(gfx::ICanvas& canvas) const = 0;
	virtual void Move(double dx, double dy) = 0;

	virtual ~IShapeStrategy() = default;
};
} // namespace shapes