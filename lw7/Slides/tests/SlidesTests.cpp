#include "../src/parse/CommandController.h"
#include "../src/canvas/SVGCanvas.h"
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

	// Read the generated SVG file
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

	// Clean up
	std::remove("test_output.svg");
}
