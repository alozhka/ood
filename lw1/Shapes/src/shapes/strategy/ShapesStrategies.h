#pragma once
#include "../Point.h"
#include "IShapeStrategy.h"

namespace shapes
{
class CircleStrategy final : public IShapeStrategy
{
public:
	CircleStrategy(double dx, double dy, double radius)
		: m_p(dx, dy)
		, m_radius(radius)
	{
	}

	void Draw(gfx::ICanvas& canvas) const override
	{
		canvas.DrawEclipse(m_p.x, m_p.y, m_radius, m_radius);
	}

	void Move(double dx, double dy) override
	{
		m_p.x += dx;
		m_p.y += dy;
	}

	std::string GetType() const override
	{
		return "circle";
	}

	std::string GetDescription() const override
	{
		return std::to_string(m_p.x) + " " + std::to_string(m_p.y) + " " + std::to_string(m_radius);
	}

private:
	Point m_p;
	double m_radius;
};

class RectangleStrategy final : public IShapeStrategy
{
public:
	RectangleStrategy(double left, double top, double width, double height)
		: m_leftTop(left, top)
		, m_width(width)
		, m_height(height)
	{
	}

	void Draw(gfx::ICanvas& canvas) const override
	{
		canvas.MoveTo(m_leftTop.x, m_leftTop.y);
		canvas.LineTo(m_leftTop.x + m_width, m_leftTop.y);
		canvas.MoveTo(m_leftTop.x + m_width, m_leftTop.y + m_height);
		canvas.LineTo(m_leftTop.x, m_leftTop.y + m_height);
		canvas.MoveTo(m_leftTop.x, m_leftTop.y);
	}

	void Move(double dx, double dy) override
	{
		m_leftTop.x += dx;
		m_leftTop.y += dy;
	}

	std::string GetType() const override
	{
		return "rectangle";
	}

	std::string GetDescription() const override
	{
		return std::to_string(m_leftTop.x) + " " + std::to_string(m_leftTop.y) + " "
			+ std::to_string(m_width) + " " + std::to_string(m_height);
	}

private:
	Point m_leftTop;
	double m_width, m_height;
};

class TriangleStrategy final : public IShapeStrategy
{
public:
	TriangleStrategy(double x1, double y1, double x2, double y2, double x3, double y3)
		: m_p1(x1, y1)
		, m_p2(x2, y2)
		, m_p3(x3, y3)
	{
	}

	void Draw(gfx::ICanvas& canvas) const override
	{
		canvas.MoveTo(m_p1.x, m_p1.y);
		canvas.LineTo(m_p2.x, m_p2.y);
		canvas.MoveTo(m_p3.x, m_p3.y);
	}

	void Move(double dx, double dy) override
	{
		m_p1.x += dx;
		m_p1.y += dy;
		m_p2.x += dx;
		m_p2.y += dy;
		m_p3.x += dx;
		m_p3.y += dy;
	}

	std::string GetType() const override
	{
		return "triangle";
	}

	std::string GetDescription() const override
	{
		return std::to_string(m_p1.x) + " " + std::to_string(m_p1.y) + " "
			+ std::to_string(m_p2.x) + " " + std::to_string(m_p2.y) + " "
			+ std::to_string(m_p3.x) + " " + std::to_string(m_p3.y);
	}

private:
	Point m_p1, m_p2, m_p3;
};

class TextStrategy final : public IShapeStrategy
{
public:
	TextStrategy(double left, double top, double size, const std::string& text)
		: m_p(left, top)
		, m_size(size)
		, m_text(text)
	{
	}

	void Draw(gfx::ICanvas& canvas) const override
	{
		canvas.DrawText(m_p.x, m_p.y, m_size, m_text);
	}

	void Move(double dx, double dy) override
	{
		m_p.x += dx;
		m_p.y += dy;
	}

	std::string GetType() const override
	{
		return "text";
	}

	std::string GetDescription() const override
	{
		return std::to_string(m_p.x) + " " + std::to_string(m_p.y) + " "
			+ std::to_string(m_size) + " " + m_text;
	}

private:
	Point m_p;
	double m_size;
	std::string m_text;
};
} // namespace shapes