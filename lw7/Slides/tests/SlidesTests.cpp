#include "../src/canvas/SVGCanvas.h"
#include "../src/parse/CommandController.h"
#include "../src/shape/ShapeGroup.h"
#include "../src/shape/types/Ellipse.h"
#include "../src/shape/types/Rectangle.h"
#include "../src/shape/types/Triangle.h"
#include "gtest/gtest.h"

class SlidesTests : public testing::Test
{
protected:
	void SetupInput(const std::string& inputData)
	{
		input.str(inputData);
		input.clear();
	}

	std::istringstream input;
	std::ostringstream output;
	CommandController controller{ input, output };
};

TEST_F(SlidesTests, InsertsShapes)
{
	SetupInput("InsertShape rectangle 0xff0000ff 0x00ff0080 350 200 100 120\n"
			   "InsertShape ellipse 0xff4010ff 0x0b78fa80 600 610 70 80\n"
			   "InsertShape triangle 0x00ffffee 0x1920aaa0 430 400 100 200\n"
			   "List\n");

	controller.Run();

	EXPECT_EQ("1. Type: rectangle; Color: outline #ff0000ff enabled, inline #00ff0080 enabled; Frame: left: 350, top: 200, width: 100, height: 120\n"
			  "2. Type: ellipse; Color: outline #ff4010ff enabled, inline #0b78fa80 enabled; Frame: left: 600, top: 610, width: 70, height: 80\n"
			  "3. Type: triangle; Color: outline #00ffffee enabled, inline #1920aaa0 enabled; Frame: left: 430, top: 400, width: 100, height: 200\n",
		output.str());
}

TEST_F(SlidesTests, TransformsShape)
{
	SetupInput("InsertShape rectangle 0xff0000ff 0x00ff0080 350 200 100 120\n"
			   "List\n"
			   "TransformShape 1 200 210 50 60\n"
			   "List\n");

	controller.Run();

	EXPECT_EQ(
		"1. Type: rectangle; Color: outline #ff0000ff enabled, inline #00ff0080 enabled; Frame: left: 350, top: 200, width: 100, height: 120\n"
		"1. Type: rectangle; Color: outline #ff0000ff enabled, inline #00ff0080 enabled; Frame: left: 200, top: 210, width: 50, height: 60\n",
		output.str());
}

TEST_F(SlidesTests, ShapesGroupHasCommonFrame)
{
	SetupInput("InsertShape ellipse 0xff4010ff 0x0b78fa80 600 610 70 80\n"
			   "InsertShape rectangle 0xff0000ff 0x00ff0080 0 0 180 120\n"
			   "InsertShape rectangle 0xff4010ff 0x0b78fa80 100 610 70 80\n"
			   "InsertShape triangle 0x00ffffee 0x1920aaa0 430 400 100 200\n"
			   "GroupShapes 2 3\n"
			   "List\n");

	controller.Run();

	EXPECT_EQ(
		"1. Type: ellipse; Color: outline #ff4010ff enabled, inline #0b78fa80 enabled; Frame: left: 600, top: 610, width: 70, height: 80\n"
		"2. Type: triangle; Color: outline #00ffffee enabled, inline #1920aaa0 enabled; Frame: left: 430, top: 400, width: 100, height: 200\n"
		"3. Type: group; Color: outline none, inline none; Frame: left: 0, top: 0, width: 180, height: 690\n",
		output.str());
}

TEST_F(SlidesTests, ShapesGroupShowsColor)
{
	SetupInput("InsertShape rectangle 0xff4010ff 0x0b78fa80 0 0 180 120\n"
			   "InsertShape rectangle 0xff4010ff 0x0b78fa80 100 610 70 80\n"
			   "GroupShapes 1 2\n"
			   "List\n");

	controller.Run();

	EXPECT_EQ(
		"1. Type: group; Color: outline #ff4010ff enabled, inline #0b78fa80 enabled; Frame: left: 0, top: 0, width: 180, height: 690\n",
		output.str());
}

TEST_F(SlidesTests, EmptyGroupShowsNoFrame)
{
	SetupInput("GroupShapes \n"
			   "List\n");

	controller.Run();

	EXPECT_EQ(
		"1. Type: group; Color: outline none, inline none; Frame: none\n",
		output.str());
}

TEST_F(SlidesTests, ShapesGroupSetsStyleToEveryShape)
{
	SetupInput("InsertShape rectangle 0xff0000ff 0x00ff0080 0 0 180 120\n"
			   "InsertShape rectangle 0xff4010ff 0x0b78fa80 100 610 70 80\n"
			   "GroupShapes 1 2\n"
			   "List\n"
			   "SetFillStyle 1 0xff4010ff enabled\n"
			   "List\n"
			   "SetLineStyle 1 0x0b78fa80 disabled\n"
			   "List\n");

	controller.Run();

	EXPECT_EQ(
		"1. Type: group; Color: outline none, inline none; Frame: left: 0, top: 0, width: 180, height: 690\n"
		"1. Type: group; Color: outline none, inline #ff4010ff enabled; Frame: left: 0, top: 0, width: 180, height: 690\n"
		"1. Type: group; Color: outline #0b78fa80 disabled, inline #ff4010ff enabled; Frame: left: 0, top: 0, width: 180, height: 690\n",
		output.str());
}

TEST_F(SlidesTests, ShapesGroupTransformsProportionally)
{
	SetupInput("InsertShape rectangle 0xff0000ff 0x00ff0080 0 0 100 100\n"
			   "InsertShape rectangle 0xff0000ff 0x00ff0080 100 100 100 100\n"
			   "GroupShapes 1 2\n"
			   "List\n"
			   "TransformShape 1 0 0 400 400\n"
			   "List\n");

	controller.Run();

	EXPECT_EQ(
		"1. Type: group; Color: outline #ff0000ff enabled, inline #00ff0080 enabled; Frame: left: 0, top: 0, width: 200, height: 200\n"
		"1. Type: group; Color: outline #ff0000ff enabled, inline #00ff0080 enabled; Frame: left: 0, top: 0, width: 400, height: 400\n",
		output.str());
}

TEST_F(SlidesTests, ExportsShapesToSVG)
{
	SetupInput("InsertShape rectangle 0x000000ff 0xffffffff 100 100 200 150\n"
			   "InsertShape ellipse 0xff0000ff 0x00ff00ff 400 200 100 80\n"
			   "InsertShape triangle 0x0000ffff 0xffff00ff 200 350 150 100\n"
			   "Export test_output.svg\n");

	controller.Run();

	std::ifstream file("test_output.svg");
	ASSERT_TRUE(file.is_open());
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string expected =
		R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="1920" height="1200">
<polygon points="100,100 300,100 300,250 100,250 100,100" fill="#ffffff" stroke="#000000" stroke-width="1" />
<ellipse cx="450" cy="240" rx="50" ry="40" fill="#00ff00" stroke="#ff0000" stroke-width="1" />
<polygon points="200,450 275,350 350,450 200,450" fill="#ffff00" stroke="#0000ff" stroke-width="1" />
</svg>
)";
	EXPECT_EQ(expected, buffer.str());
	std::remove("test_output.svg");
}

// Clone tests
TEST_F(SlidesTests, CloneRectangle)
{
	Frame frame{ 350, 200, 100, 120 };
	Style lineStyle;
	lineStyle.SetColor(0xff0000ff);
	lineStyle.Enable(true);
	Style fillStyle;
	fillStyle.SetColor(0x00ff0080);
	fillStyle.Enable(true);

	auto rectangle = std::make_shared<Rectangle>(frame, lineStyle, fillStyle);
	auto clonedRectangle = rectangle->Clone();

	EXPECT_NE(rectangle.get(), clonedRectangle.get());
	EXPECT_EQ(rectangle->GetType(), clonedRectangle->GetType());
	EXPECT_EQ(rectangle->GetFrame()->left, clonedRectangle->GetFrame()->left);
	EXPECT_EQ(rectangle->GetFrame()->top, clonedRectangle->GetFrame()->top);
	EXPECT_EQ(rectangle->GetFrame()->width, clonedRectangle->GetFrame()->width);
	EXPECT_EQ(rectangle->GetFrame()->height, clonedRectangle->GetFrame()->height);
	EXPECT_EQ(rectangle->GetLineStyle()->GetColor(), clonedRectangle->GetLineStyle()->GetColor());
	EXPECT_EQ(rectangle->GetFillStyle()->GetColor(), clonedRectangle->GetFillStyle()->GetColor());

	clonedRectangle->SetFrame({ 100, 100, 50, 50 });
	EXPECT_NE(rectangle->GetFrame()->left, clonedRectangle->GetFrame()->left);
	EXPECT_NE(rectangle->GetFrame()->width, clonedRectangle->GetFrame()->width);
}

TEST_F(SlidesTests, CloneTriangle)
{
	Frame frame{ 430, 400, 100, 200 };
	Style lineStyle;
	lineStyle.SetColor(0x00ffffee);
	lineStyle.Enable(true);
	Style fillStyle;
	fillStyle.SetColor(0x1920aaa0);
	fillStyle.Enable(true);

	auto triangle = std::make_shared<Triangle>(frame, lineStyle, fillStyle);
	auto clonedTriangle = triangle->Clone();

	EXPECT_NE(triangle.get(), clonedTriangle.get());
	EXPECT_EQ(triangle->GetType(), clonedTriangle->GetType());
	EXPECT_EQ(triangle->GetFrame()->left, clonedTriangle->GetFrame()->left);
	EXPECT_EQ(triangle->GetFrame()->top, clonedTriangle->GetFrame()->top);
	EXPECT_EQ(triangle->GetFrame()->width, clonedTriangle->GetFrame()->width);
	EXPECT_EQ(triangle->GetFrame()->height, clonedTriangle->GetFrame()->height);

	clonedTriangle->SetLineStyle(0xff0000ff, true);
	EXPECT_NE(triangle->GetLineStyle()->GetColor(), clonedTriangle->GetLineStyle()->GetColor());
}

TEST_F(SlidesTests, CloneEllipse)
{
	Frame frame{ 600, 610, 70, 80 };
	Style lineStyle;
	lineStyle.SetColor(0xff4010ff);
	lineStyle.Enable(true);
	Style fillStyle;
	fillStyle.SetColor(0x0b78fa80);
	fillStyle.Enable(true);

	auto ellipse = std::make_shared<Ellipse>(frame, lineStyle, fillStyle);
	auto clonedEllipse = ellipse->Clone();

	EXPECT_NE(ellipse.get(), clonedEllipse.get());
	EXPECT_EQ(ellipse->GetType(), clonedEllipse->GetType());
	EXPECT_EQ(ellipse->GetFrame()->left, clonedEllipse->GetFrame()->left);
	EXPECT_EQ(ellipse->GetFrame()->top, clonedEllipse->GetFrame()->top);
	EXPECT_EQ(ellipse->GetFrame()->width, clonedEllipse->GetFrame()->width);
	EXPECT_EQ(ellipse->GetFrame()->height, clonedEllipse->GetFrame()->height);

	clonedEllipse->SetFillStyle(0xff00ffff, true);
	EXPECT_NE(ellipse->GetFillStyle()->GetColor(), clonedEllipse->GetFillStyle()->GetColor());
}

TEST_F(SlidesTests, CloneShapeGroup)
{
	Frame frame1{ 0, 0, 100, 100 };
	Frame frame2{ 100, 100, 100, 100 };
	Style lineStyle;
	lineStyle.SetColor(0xff0000ff);
	lineStyle.Enable(true);
	Style fillStyle;
	fillStyle.SetColor(0x00ff0080);
	fillStyle.Enable(true);

	auto rect1 = std::make_shared<Rectangle>(frame1, lineStyle, fillStyle);
	auto rect2 = std::make_shared<Rectangle>(frame2, lineStyle, fillStyle);

	std::vector<std::shared_ptr<IShape>> shapes{ rect1, rect2 };
	auto group = std::make_shared<ShapeGroup>(shapes);

	auto clonedGroup = group->Clone();

	EXPECT_NE(group.get(), clonedGroup.get());
	EXPECT_EQ(group->GetType(), clonedGroup->GetType());
	EXPECT_EQ(group->GetFrame()->left, clonedGroup->GetFrame()->left);
	EXPECT_EQ(group->GetFrame()->top, clonedGroup->GetFrame()->top);
	EXPECT_EQ(group->GetFrame()->width, clonedGroup->GetFrame()->width);
	EXPECT_EQ(group->GetFrame()->height, clonedGroup->GetFrame()->height);

	clonedGroup->SetFillStyle(0xff00ffff, true);
	EXPECT_NE(group->GetFillStyle()->GetColor(), clonedGroup->GetFillStyle()->GetColor());

	EXPECT_EQ(rect1->GetFillStyle()->GetColor(), 0x00ff0080);
	EXPECT_EQ(rect2->GetFillStyle()->GetColor(), 0x00ff0080);
}

TEST_F(SlidesTests, CloneNestedShapeGroup)
{
	// Create nested group structure
	Frame frame1{ 0, 0, 50, 50 };
	Frame frame2{ 50, 50, 50, 50 };
	Frame frame3{ 100, 100, 50, 50 };

	Style lineStyle;
	lineStyle.SetColor(0xff0000ff);
	lineStyle.Enable(true);
	Style fillStyle;
	fillStyle.SetColor(0x00ff0080);
	fillStyle.Enable(true);

	auto rect1 = std::make_shared<Rectangle>(frame1, lineStyle, fillStyle);
	auto rect2 = std::make_shared<Rectangle>(frame2, lineStyle, fillStyle);
	auto rect3 = std::make_shared<Rectangle>(frame3, lineStyle, fillStyle);

	std::vector<std::shared_ptr<IShape>> innerShapes{ rect1, rect2 };
	auto innerGroup = std::make_shared<ShapeGroup>(innerShapes);

	std::vector<std::shared_ptr<IShape>> outerShapes{ innerGroup, rect3 };
	auto outerGroup = std::make_shared<ShapeGroup>(outerShapes);

	auto clonedOuterGroup = outerGroup->Clone();

	EXPECT_NE(innerGroup.get(), clonedOuterGroup.get());
	EXPECT_EQ(outerGroup->GetType(), clonedOuterGroup->GetType());
	EXPECT_EQ(outerGroup->GetFrame()->left, clonedOuterGroup->GetFrame()->left);
	EXPECT_EQ(outerGroup->GetFrame()->top, clonedOuterGroup->GetFrame()->top);
	EXPECT_EQ(outerGroup->GetFrame()->width, clonedOuterGroup->GetFrame()->width);
	EXPECT_EQ(outerGroup->GetFrame()->height, clonedOuterGroup->GetFrame()->height);

	clonedOuterGroup->SetLineStyle(0x0000ffff, true);

	EXPECT_EQ(rect1->GetLineStyle()->GetColor(), 0xff0000ff);
	EXPECT_EQ(rect2->GetLineStyle()->GetColor(), 0xff0000ff);
	EXPECT_EQ(rect3->GetLineStyle()->GetColor(), 0xff0000ff);

	EXPECT_EQ(clonedOuterGroup->GetLineStyle()->GetColor(), 0x0000ffff);
}
