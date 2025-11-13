#include "../src/parse/CommandController.h"
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

	EXPECT_EQ("1. Type: rectangle; Color: outline #ff0000ff, inline #00ff0080; Frame: left: 350, top: 200, width: 100, height: 120\n"
			  "2. Type: ellipse; Color: outline #ff4010ff, inline #0b78fa80; Frame: left: 600, top: 610, width: 70, height: 80\n"
			  "3. Type: triangle; Color: outline #00ffffee, inline #1920aaa0; Frame: left: 430, top: 400, width: 100, height: 200\n",
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
		"1. Type: rectangle; Color: outline #ff0000ff, inline #00ff0080; Frame: left: 350, top: 200, width: 100, height: 120\n"
		"1. Type: rectangle; Color: outline #ff0000ff, inline #00ff0080; Frame: left: 200, top: 210, width: 50, height: 60\n",
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
		"1. Type: ellipse; Color: outline #ff4010ff, inline #0b78fa80; Frame: left: 600, top: 610, width: 70, height: 80\n"
		"2. Type: triangle; Color: outline #00ffffee, inline #1920aaa0; Frame: left: 430, top: 400, width: 100, height: 200\n"
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
		"1. Type: group; Color: outline #ff4010ff, inline #0b78fa80; Frame: left: 0, top: 0, width: 180, height: 690\n",
		output.str());
}
