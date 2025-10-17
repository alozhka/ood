#pragma once
#include "Color.h"
#include "Point.h"

class ICanvas
{
public:
	virtual void SetColor(Color color) = 0;
	virtual void DrawLine(double fromX, double fromY, double toX, double toY) = 0;
	virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
	virtual ~ICanvas() = default;
};