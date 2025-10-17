#include "../src/shapes/Ellipse.h"
#include "../src/shapes/Rectangle.h"
#include "../src/shapes/RegularPolygon.h"
#include "../src/shapes/Triangle.h"

#include "gtest/gtest.h"
class ShapesTests : public testing::Test
{
};

TEST_F(ShapesTests, CreatesTriangle)
{
	Point p1{ 30, 20 }, p2{ 100, 60 }, p3{ 10, 40 };
	Color color = Color::Pink;

	Triangle triangle{ color, p1, p2, p3 };

	EXPECT_EQ(color, triangle.GetColor());
	EXPECT_EQ(p1, triangle.GetVertex1());
	EXPECT_EQ(p2, triangle.GetVertex2());
	EXPECT_EQ(p3, triangle.GetVertex3());
}

TEST_F(ShapesTests, CreatesRectangle)
{
	Point p{ 30, 20 };
	double width = 10, height = 5;
	Color color = Color::Pink;

	Rectangle rectangle{ color, p, width, height };

	EXPECT_EQ(color, rectangle.GetColor());
	EXPECT_EQ(p, rectangle.GetLeftTop());
	EXPECT_EQ(width, rectangle.GetWidth());
	EXPECT_EQ(height, rectangle.GetHeight());
}

TEST_F(ShapesTests, CreatesEllipse)
{
	Point p{ 30, 20 };
	double rx = 10, ry = 5;
	Color color = Color::Pink;

	Ellipse ellipse{ color, p, rx, ry };

	EXPECT_EQ(color, ellipse.GetColor());
	EXPECT_EQ(p, ellipse.GetCenter());
	EXPECT_EQ(rx, ellipse.GetHorizontalRadius());
	EXPECT_EQ(ry, ellipse.GetVerticalRadius());
}

TEST_F(ShapesTests, CreatesRegularPolygon)
{
	Point p{ 30, 20 };
	unsigned int vertexesCount = 6;
	double radius = 15;
	Color color = Color::Pink;

	RegularPolygon polygon{ color, p, vertexesCount, radius };

	EXPECT_EQ(color, polygon.GetColor());
	EXPECT_EQ(p, polygon.GetCenter());
	EXPECT_EQ(radius, polygon.GetRadius());
	EXPECT_EQ(vertexesCount, polygon.GetVertexCount());
}