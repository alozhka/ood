#include "../src/commands/ShapesController.h"
#include "../src/commands/ShapesFactory.h"

#include <gtest/gtest.h>

class ShapeTests : public testing::Test
{
};

TEST_F(ShapeTests, ShapeCanBeConstructedFromStream)
{
	std::stringstream ss("200 250 45.55");
	auto strategy = ShapesFactory::CreateFromStream("circle", ss);

	std::string id = "sh1";
	gfx::Color color = gfx::Color::FromHex("eeefff");
	Shape shape(id, color, (std::move(strategy)));

	EXPECT_EQ(id, shape.GetId());
	EXPECT_EQ(color, shape.GetColor());
	EXPECT_EQ("circle sh1 #eeefff 200 250 45.55", shape.GetDescription());
}