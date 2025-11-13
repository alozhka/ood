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
			   "List");

	controller.Run();

	EXPECT_EQ("1. Type: rectangle; Color: outline #ff0000ff, inline #00ff0080; Frame: left: 200, top: 210, width: 50, height: 60\n",
		output.str());
}