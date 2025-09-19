#pragma once

#include <string>

namespace gfx
{
struct Color
{
	Color(uint8_t red, uint8_t green, uint8_t blue)
		: R(red)
		, G(green)
		, B(blue)
	{
	}

	uint8_t R, G, B;
};

class ICanvas
{
public:
	virtual void SetColor(Color c) = 0;
	virtual void MoveTo(double x, double y) = 0;
	virtual void LineTo(double x, double y) = 0;
	virtual void DrawEclipse(double cx, double cy, double rx, double ry) = 0;
	virtual void DrawText(double left, double top, double fontSize, const std::string& text) = 0;

	virtual ~ICanvas() = default;
};
} // namespace gfx