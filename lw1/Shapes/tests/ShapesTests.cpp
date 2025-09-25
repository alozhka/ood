#include "../src/commands/ShapesController.h"
#include "../src/commands/ShapesFactory.h"

#include <gtest/gtest.h>

class ShapeTests : public testing::Test
{
protected:
	static Shape CreateShape(const std::string& id, gfx::Color color, const std::string& type, const std::string& params)
	{
		auto strategy = CreateStrategy(type, params);
		return Shape{ id, color, (std::move(strategy)) };
	}
	static std::unique_ptr<shapes::IShapeStrategy> CreateStrategy(const std::string& type, const std::string& params)
	{
		std::stringstream ss(params);
		return ShapesFactory::CreateFromStream(type, ss);
	}
};

TEST_F(ShapeTests, ShapeCanBeConstructedFromStream)
{
	std::string id = "sh1";
	gfx::Color color = gfx::Color::FromHex("eeefff");
	Shape shape = CreateShape(id, color, "circle", "200 250 45.55");

	EXPECT_EQ(id, shape.GetId());
	EXPECT_EQ(color, shape.GetColor());
	EXPECT_EQ("circle sh1 #eeefff 200 250 45.55", shape.GetDescription());
}

TEST_F(ShapeTests, ShapeCanBeMoved)
{
	std::string id = "sh1";
	gfx::Color color = gfx::Color::FromHex("eeefff");
	Shape shape = CreateShape(id, color, "circle", "200 250 45.55");

	EXPECT_EQ(id, shape.GetId());
	EXPECT_EQ(color, shape.GetColor());
	EXPECT_EQ("circle sh1 #eeefff 200 250 45.55", shape.GetDescription());

	shape.Move(120, 130);

	EXPECT_EQ("circle sh1 #eeefff 320 380 45.55", shape.GetDescription());
}

TEST_F(ShapeTests, ShapeCanChangeColor)
{
	std::string id = "sh1";
	gfx::Color color = gfx::Color::FromHex("eeefff");
	gfx::Color newColor = gfx::Color::FromHex("333aaa");
	Shape shape = CreateShape(id, color, "circle", "200 250 45.55");

	EXPECT_EQ(id, shape.GetId());
	EXPECT_EQ(color, shape.GetColor());
	EXPECT_EQ("circle sh1 #eeefff 200 250 45.55", shape.GetDescription());

	shape.SetColor(newColor);

	EXPECT_EQ(newColor, shape.GetColor());
	EXPECT_EQ("circle sh1 #333aaa 200 250 45.55", shape.GetDescription());
}

TEST_F(ShapeTests, ShapeCanChangeStrategy)
{
	std::unique_ptr<shapes::IShapeStrategy> newStrategy = CreateStrategy("rectangle", "150 100 20 30");
	std::string id = "sh1";
	gfx::Color color = gfx::Color::FromHex("eeefff");
	Shape shape = CreateShape(id, color, "circle", "200 250 45.55");

	EXPECT_EQ(id, shape.GetId());
	EXPECT_EQ(color, shape.GetColor());
	EXPECT_EQ("circle sh1 #eeefff 200 250 45.55", shape.GetDescription());

	shape.SetStrategy(std::move(newStrategy));

	EXPECT_EQ("rectangle sh1 #eeefff 150 100 20 30", shape.GetDescription());
}