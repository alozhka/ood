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
    CommandController controller{input, output};
};

TEST_F(SlidesTests, InsertsShapes)
{
    SetupInput("InsertShape rectangle 350 200 100 120 #ff0000FF #00ff0080\nList\n");

    controller.Run();

    EXPECT_EQ("1. Type: rectangle; Params: x: 350, y: 200, width: 100, height: 120\n", output.str());
}
