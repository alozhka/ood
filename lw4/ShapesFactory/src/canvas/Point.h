#pragma once

struct Point
{
	Point(double x, double y)
		: x(x)
		, y(y)
	{
	}
	double x, y;

	bool operator==(const Point& other) const
	{
		return x == other.x && y == other.y;
	}
};