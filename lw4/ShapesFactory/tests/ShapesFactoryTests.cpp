#include "../src/people/Client.h"
#include "../src/people/Designer.h"
#include "../src/people/Painter.h"
#include "../src/picture/PictureDraft.h"
#include "../src/shapes/Ellipse.h"
#include "../src/shapes/Rectangle.h"
#include "../src/shapes/RegularPolygon.h"
#include "../src/shapes/ShapeFactory.h"
#include "../src/shapes/Triangle.h"
#include "TestCanvas.h"

#include "gtest/gtest.h"
#include <sstream>

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

class ShapeFactoryTests : public testing::Test
{
protected:
	ShapeFactory factory;
};

TEST_F(ShapeFactoryTests, CreatesRectangleFromDescription)
{
	auto shape = factory.CreateShape("rectangle red 10 20 30 40");

	EXPECT_EQ(shape->GetColor(), Color::Red);

	auto* rect = dynamic_cast<Rectangle*>(shape.get());
	ASSERT_NE(rect, nullptr);
	EXPECT_EQ(rect->GetLeftTop(), Point(10, 20));
	EXPECT_EQ(rect->GetWidth(), 30);
	EXPECT_EQ(rect->GetHeight(), 40);
}

TEST_F(ShapeFactoryTests, CreatesTriangleFromDescription)
{
	auto shape = factory.CreateShape("triangle green 1 2 3 4 5 6");

	EXPECT_EQ(shape->GetColor(), Color::Green);

	auto* tri = dynamic_cast<Triangle*>(shape.get());
	ASSERT_NE(tri, nullptr);
	EXPECT_EQ(tri->GetVertex1(), Point(1, 2));
	EXPECT_EQ(tri->GetVertex2(), Point(3, 4));
	EXPECT_EQ(tri->GetVertex3(), Point(5, 6));
}

TEST_F(ShapeFactoryTests, ThrowsOnInvalidShapeType)
{
	EXPECT_THROW(factory.CreateShape("invalid red 1 2 3 4"), std::invalid_argument);
}

class PictureDraftTests : public testing::Test
{
};

TEST_F(PictureDraftTests, AddsAndRetrievesShapes)
{
	PictureDraft draft;

	draft.AddShape(std::make_unique<Rectangle>(Color::Red, Point(0, 0), 10, 20));
	draft.AddShape(std::make_unique<Ellipse>(Color::Blue, Point(5, 5), 3, 4));

	EXPECT_EQ(draft.GetShapesCount(), 2);
	EXPECT_EQ(draft.GetShape(0).GetColor(), Color::Red);
	EXPECT_EQ(draft.GetShape(1).GetColor(), Color::Blue);
}

TEST_F(PictureDraftTests, ThrowsOnInvalidIndex)
{
	PictureDraft draft;
	draft.AddShape(std::make_unique<Rectangle>(Color::Red, Point(0, 0), 10, 20));

	EXPECT_THROW(draft.GetShape(10), std::range_error);
}

class DesignerTests : public testing::Test
{
protected:
	ShapeFactory factory;
};

TEST_F(DesignerTests, CreatesDraftFromInput)
{
	Designer designer(factory);
	std::istringstream input("rectangle red 10 20 30 40\ntriangle blue 1 2 3 4 5 6\n");

	PictureDraft draft = designer.CreateDraft(input);

	EXPECT_EQ(draft.GetShapesCount(), 2);
	EXPECT_EQ(draft.GetShape(0).GetColor(), Color::Red);
	EXPECT_EQ(draft.GetShape(1).GetColor(), Color::Blue);
}

TEST_F(DesignerTests, SkipsInvalidLines)
{
	Designer designer(factory);
	std::istringstream input("rectangle red 10 20 30 40\ninvalid line\nellipse green 5 5 2 3\n");

	PictureDraft draft = designer.CreateDraft(input);

	EXPECT_EQ(draft.GetShapesCount(), 2);
}

class PainterTests : public testing::Test
{
};

TEST_F(PainterTests, DrawsPictureOnCanvas)
{
	PictureDraft draft;
	TestCanvas canvas;
	Painter painter;
	draft.AddShape(std::make_unique<RegularPolygon>(Color::Red, Point(0, 0), 3, 20));
	draft.AddShape(std::make_unique<Ellipse>(Color::Blue, Point(5, 5), 3, 4));

	painter.DrawPicture(draft, canvas);

	std::vector<std::string>& results = canvas.GetResults();
	EXPECT_EQ("set color", results[0]);
	EXPECT_EQ("draw line", results[1]);
	EXPECT_EQ("draw line", results[2]);
	EXPECT_EQ("draw line", results[3]);
	EXPECT_EQ("set color", results[5]);
	EXPECT_EQ("draw ellipse", results[6]);
}

class ClientTests : public testing::Test
{
protected:
	ShapeFactory factory;
};

TEST_F(ClientTests, HandlesCompleteWorkflow)
{
	Designer designer(factory);
	Client client(designer);
	Painter painter;
	TestCanvas canvas;
	std::istringstream input("rectangle red 10 20 30 40\nellipse blue 5 5 2 3\n");

	client.HandleCommand(input, canvas, painter);

	std::vector<std::string>& results = canvas.GetResults();
	EXPECT_EQ("set color", results[0]);
	EXPECT_EQ("draw line", results[1]);
	EXPECT_EQ("draw line", results[2]);
	EXPECT_EQ("draw line", results[3]);
	EXPECT_EQ("draw line", results[4]);
	EXPECT_EQ("set color", results[5]);
	EXPECT_EQ("draw ellipse", results[6]);
}