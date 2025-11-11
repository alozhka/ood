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
	SetupInput("InsertShape rectangle 0xff0000FF 0x00ff0080 350 200 100 120\n"
			   "List\n");

	controller.Run();

	EXPECT_EQ("1. Type: rectangle; Color: outline #ff0000ff, inline #00ff0080; Frame: left: 350, top: 200, width: 100, height: 120\n", output.str());
}
