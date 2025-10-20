#include "../src/parse/CommandController.h"
#include "gtest/gtest.h"

class CommandControllerTests : public testing::TestWithParam<std::string>, public testing::TestResult
{
protected:
    CommandControllerTests() : input{GetParam()}, controller{input}
    {
    }

    std::istringstream input;
    CommandController controller;
};

TEST_P(CommandControllerTests, CorrectlryParsesCommands)
{
    controller.HandleCommand();
}

INSTANTIATE_TEST_SUITE_P(
    CommandControllerTests,
    CommandControllerTests_CorrectlryParsesCommands_Test,
    testing::Values("Help"));
