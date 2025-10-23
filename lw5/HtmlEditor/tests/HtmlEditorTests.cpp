#include "../src/CommandController.h"
#include "../src/Menu.h"
#include "gtest/gtest.h"

class CommandControllerTests : public testing::Test
{
protected:
	void SetupInput(const std::string& inputData)
	{
		input.str(inputData);
		input.clear();
	}

	std::istringstream input;
	std::ostringstream output;
};

TEST_F(CommandControllerTests, PrintsHelp)
{
	SetupInput("Help\nExit");
	CommandController controller(input, output);

	controller.Run();

	EXPECT_EQ("Commands list:\n", output.str());
}