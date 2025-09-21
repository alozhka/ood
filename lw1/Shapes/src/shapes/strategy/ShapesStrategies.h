#pragma once
#include "IShapeStrategy.h"

namespace shapes
{
class CircleStrategy final : public IShapeStrategy
{
public:
	CircleStrategy(double dx, double dy, double radius)
		: m_dx(dx)
		, m_dy(dy)
		, m_radius(radius)
	{
	}

	void Draw(gfx::ICanvas& canvas) const override
	{
		canvas.DrawEclipse(m_dx, m_dy, m_radius, m_radius);
	}

	void Move(double dx, double dy) override
	{
		m_dx += m_dx;
		m_dy += m_dy;
	}

private:
	double m_dx, m_dy, m_radius;
};

class RectangleStrategy final : public IShapeStrategy
{
public:
	RectangleStrategy(double left, double top, double width, double height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
	{
	}

	void Draw(gfx::ICanvas& canvas) const override
	{
		canvas.MoveTo(m_left, m_top);
		canvas.LineTo(m_left + m_width, m_top);
		canvas.MoveTo(m_left + m_width, m_top + m_height);
		canvas.LineTo(m_left, m_top + m_height);
		canvas.MoveTo(m_left, m_top);
	}

	void Move(double dx, double dy) override
	{
		m_left += dx;
		m_top += dy;
	}

private:
	double m_left, m_top, m_width, m_height;
};

class TriangleStrategy final : public IShapeStrategy
{
public:
	TriangleStrategy(double x1, double y1, double x2, double y2, double x3, double y3)
		: m_x1(x1)
		, m_y1(y1)
		, m_x2(x2)
		, m_y2(y2)
		, m_x3(x3)
		, m_y3(y3)
	{
	}

	void Draw(gfx::ICanvas& canvas) const override
	{
		canvas.MoveTo(m_x1, m_y1);
		canvas.LineTo(m_x2, m_y2);
		canvas.MoveTo(m_x3, m_y3);
		canvas.LineTo(m_x1, m_y1);
	}

	void Move(double dx, double dy) override
	{
		m_x1 += dx;
		m_y1 += dy;
		m_x2 += dx;
		m_y2 += dy;
		m_x3 += dx;
		m_y3 += dy;
	}

private:
	double m_x1, m_y1, m_x2, m_y2, m_x3, m_y3;
};
} // namespace shapes