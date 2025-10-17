#pragma once
#include "Shape.h"
#include <numbers>

class RegularPolygon : public Shape
{
public:
	explicit RegularPolygon(Color color, Point center, unsigned int vertexCount, double radius)
		: Shape(color)
		, m_center(center)
		, m_vertexCount(vertexCount)
		, m_radius(radius)
	{
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());

		double angleBetweenVertexes = 2 * std::numbers::pi / m_vertexCount;

		double startX = m_radius * std::cos(0) + m_center.x;
		double startY = m_radius * std::sin(0) + m_center.y;

		double prevX = startX;
		double prevY = startY;

		for (unsigned i = 0; i < m_vertexCount; ++i)
		{
			double angle = i * angleBetweenVertexes;
			double x = m_radius * std::cos(angle) + m_center.x;
			double y = m_radius * std::sin(angle) + m_center.y;

			canvas.DrawLine(prevX, prevY, x, y);
			prevX = x;
			prevY = y;
		}

		canvas.DrawLine(prevX, prevY, startX, startY);
	}

	Point GetCenter() const
	{
		return m_center;
	}

	unsigned int GetVertexCount() const
	{
		return m_vertexCount;
	}

	double GetRadius() const
	{
		return m_radius;
	}

private:
	Point m_center;
	unsigned int m_vertexCount;
	double m_radius;
};