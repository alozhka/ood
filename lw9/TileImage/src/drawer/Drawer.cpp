#include "Drawer.h"
#include <cassert>
#include <cstdlib>

namespace
{

int Sign(int value)
{
	return (0 < value) - (value < 0);
}

/**
 * ( |to.y - from.x| >= |to.x - from.x|).
 */
void DrawSteepLine(Image& image, Point from, Point to, uint32_t color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	assert(deltaY >= deltaX);

	if (from.y > to.y)
	{
		std::swap(from, to);
	}

	const int stepX = Sign(to.x - from.x);
	const int errorThreshold = deltaY + 1;
	const int deltaErr = deltaX + 1;

	int error = deltaErr / 2;

	for (Point p = from; p.y <= to.y; ++p.y)
	{
		image.SetPixel({ p.x, p.y }, color);
		assert((p.y != to.y) || (p.x == to.x));

		error += deltaErr;

		if (error >= errorThreshold)
		{
			p.x += stepX;
			error -= errorThreshold;
		}
	}
}

/**
 * (|to.y - from.x| >= |to.y - from.y|).
 */
void DrawSlopeLine(Image& image, Point from, Point to, uint32_t color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	assert(deltaX >= deltaY);

	if (from.x > to.x)
	{
		std::swap(from, to);
	}

	const int stepY = Sign(to.y - from.y);
	const int errorThreshold = deltaX + 1;
	const int deltaErr = deltaY + 1;

	int error = deltaErr / 2;

	for (Point p = from; p.x <= to.x; ++p.x)
	{
		image.SetPixel({ p.x, p.y }, color);
		assert((p.x != to.x) || (p.y == to.y));

		error += deltaErr;

		if (error >= errorThreshold)
		{
			p.y += stepY;
			error -= errorThreshold;
		}
	}
}

} // namespace

void Drawer::DrawLine(Image& image, Point from, Point to, uint32_t color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	if (deltaY > deltaX)
	{
		// отрезок крутой
		DrawSteepLine(image, from, to, color);
	}
	else
	{
		// отрезок пологий
		DrawSlopeLine(image, from, to, color);
	}
}

void Drawer::DrawCircle(Image& image, Point center, int radius, uint32_t color)
{
	if (radius <= 0)
	{
		return;
	}

	int x = 0;
	int y = radius;
	int error = 3 - 2 * radius;

	// Рисуем начальные точки
	image.SetPixel({ center.x + x, center.y + y }, color);
	image.SetPixel({ center.x - x, center.y + y }, color);
	image.SetPixel({ center.x + x, center.y - y }, color);
	image.SetPixel({ center.x - x, center.y - y }, color);
	image.SetPixel({ center.x + y, center.y + x }, color);
	image.SetPixel({ center.x - y, center.y + x }, color);
	image.SetPixel({ center.x + y, center.y - x }, color);
	image.SetPixel({ center.x - y, center.y - x }, color);

	while (y >= x)
	{
		x++;

		if (error > 0)
		{
			y--;
			error = error + 4 * (x - y) + 10;
		}
		else
		{
			error = error + 4 * x + 6;
		}

		// Рисуем 8 симметричных точек окружности
		image.SetPixel({ center.x + x, center.y + y }, color);
		image.SetPixel({ center.x - x, center.y + y }, color);
		image.SetPixel({ center.x + x, center.y - y }, color);
		image.SetPixel({ center.x - x, center.y - y }, color);
		image.SetPixel({ center.x + y, center.y + x }, color);
		image.SetPixel({ center.x - y, center.y + x }, color);
		image.SetPixel({ center.x + y, center.y - x }, color);
		image.SetPixel({ center.x - y, center.y - x }, color);
	}
}

void Drawer::FillCircle(Image& image, Point center, int radius, uint32_t color)
{
	if (radius <= 0)
	{
		return;
	}

	int x = 0;
	int y = radius;
	int error = 3 - 2 * radius;

	// Закрашиваем начальную горизонтальную линию
	DrawLine(image, { center.x - y, center.y }, { center.x + y, center.y }, color);

	while (y >= x)
	{
		x++;

		if (error > 0)
		{
			y--;
			error = error + 4 * (x - y) + 10;
		}
		else
		{
			error = error + 4 * x + 6;
		}

		DrawLine(image, { center.x - x, center.y + y }, { center.x + x, center.y + y }, color);
		DrawLine(image, { center.x - x, center.y - y }, { center.x + x, center.y - y }, color);
		DrawLine(image, { center.x - y, center.y + x }, { center.x + y, center.y + x }, color);
		DrawLine(image, { center.x - y, center.y - x }, { center.x + y, center.y - x }, color);
	}
}
