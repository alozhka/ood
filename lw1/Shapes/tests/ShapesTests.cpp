#include "../src/commands/ShapesController.h"
#include "../src/commands/ShapesFactory.h"
#include "MockCanvas.h"

#include <gtest/gtest.h>

class ShapeTests : public testing::Test
{
protected:
	static std::unique_ptr<shapes::IShapeStrategy> CreateStrategy(const std::string& type, const std::string& params)
	{
		std::stringstream ss(params);
		return ShapesFactory::CreateFromStream(type, ss);
	}

	static Shape CreateShape(const std::string& id, gfx::Color color, const std::string& type, const std::string& params)
	{
		auto strategy = CreateStrategy(type, params);
		return Shape{ id, color, (std::move(strategy)) };
	}
};

TEST_F(ShapeTests, ShapeCanBeConstructedFromStream)
{
	std::string id = "sh1";
	gfx::Color color = gfx::Color::FromHexStr("eeefff");
	Shape shape = CreateShape(id, color, "circle", "200 250 45.55");

	EXPECT_EQ(id, shape.GetId());
	EXPECT_EQ(color, shape.GetColor());
	EXPECT_EQ("circle sh1 #eeefff 200 250 45.55", shape.GetDescription());
}

TEST_F(ShapeTests, ShapeCanBeMoved)
{
	std::string id = "sh1";
	gfx::Color color = gfx::Color::FromHexStr("eeefff");
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
	gfx::Color color = gfx::Color::FromHexStr("eeefff");
	gfx::Color newColor = gfx::Color::FromHexStr("333aaa");
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
	gfx::Color color = gfx::Color::FromHexStr("eeefff");
	Shape shape = CreateShape(id, color, "circle", "200 250 45.55");

	EXPECT_EQ(id, shape.GetId());
	EXPECT_EQ(color, shape.GetColor());
	EXPECT_EQ("circle sh1 #eeefff 200 250 45.55", shape.GetDescription());

	shape.SetStrategy(std::move(newStrategy));

	EXPECT_EQ("rectangle sh1 #eeefff 150 100 20 30", shape.GetDescription());
}

class PictureTests : public testing::Test
{
protected:
	static std::unique_ptr<shapes::IShapeStrategy> CreateStrategy(const std::string& type, const std::string& params)
	{
		std::stringstream ss(params);
		return ShapesFactory::CreateFromStream(type, ss);
	}

	static std::unique_ptr<Shape> CreateShape(const std::string& id, gfx::Color color, const std::string& type, const std::string& params)
	{
		auto strategy = CreateStrategy(type, params);
		return std::make_unique<Shape>(id, color, std::move(strategy));
	}
};

TEST_F(PictureTests, PictureCanAddShape)
{
	shapes::Picture picture;
	auto shape = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");

	EXPECT_NO_THROW(picture.AddShape(std::move(shape)));

	auto shapesList = picture.ListShapes();
	EXPECT_EQ(1, shapesList.size());
	EXPECT_EQ("shape1", shapesList[0]->GetId());
}

TEST_F(PictureTests, PictureCannotAddShapeWithDuplicateId)
{
	shapes::Picture picture;
	auto shape1 = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");
	auto shape2 = CreateShape("shape1", gfx::Color::FromHexStr("000000"), "rectangle", "50 75 20 30");

	picture.AddShape(std::move(shape1));

	EXPECT_THROW(picture.AddShape(std::move(shape2)), std::runtime_error);

	auto shapesList = picture.ListShapes();
	EXPECT_EQ(1, shapesList.size());
}

TEST_F(PictureTests, PictureCanRemoveShape)
{
	shapes::Picture picture;
	auto shape = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");

	picture.AddShape(std::move(shape));
	EXPECT_EQ(1, picture.ListShapes().size());

	EXPECT_NO_THROW(picture.RemoveShape("shape1"));
	EXPECT_EQ(0, picture.ListShapes().size());
}

TEST_F(PictureTests, PictureCannotRemoveNonExistentShape)
{
	shapes::Picture picture;

	EXPECT_THROW(picture.RemoveShape("nonexistent"), std::runtime_error);
}

TEST_F(PictureTests, PictureCanMoveShape)
{
	shapes::Picture picture;
	auto shape = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");

	picture.AddShape(std::move(shape));

	EXPECT_EQ("circle shape1 #ffffff 100 150 25", picture.ListShapes()[0]->GetDescription());

	picture.MoveShape("shape1", 50, 75);

	EXPECT_EQ("circle shape1 #ffffff 150 225 25", picture.ListShapes()[0]->GetDescription());
}

TEST_F(PictureTests, PictureCannotMoveNonExistentShape)
{
	shapes::Picture picture;

	EXPECT_THROW(picture.MoveShape("nonexistent", 10, 20), std::runtime_error);
}

TEST_F(PictureTests, PictureCanMovePicture)
{
	shapes::Picture picture;
	auto shape1 = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");
	auto shape2 = CreateShape("shape2", gfx::Color::FromHexStr("000000"), "rectangle", "200 250 30 40");

	picture.AddShape(std::move(shape1));
	picture.AddShape(std::move(shape2));

	std::vector<Shape*> shapesList = picture.ListShapes();
	EXPECT_EQ("circle shape1 #ffffff 100 150 25", shapesList[0]->GetDescription());
	EXPECT_EQ("rectangle shape2 #000000 200 250 30 40", shapesList[1]->GetDescription());

	picture.MovePicture(50, 75);

	shapesList = picture.ListShapes();
	EXPECT_EQ("circle shape1 #ffffff 150 225 25", shapesList[0]->GetDescription());
	EXPECT_EQ("rectangle shape2 #000000 250 325 30 40", shapesList[1]->GetDescription());
}

TEST_F(PictureTests, ListShapesReturnsThemInOrderOfAddition)
{
	shapes::Picture picture;
	auto shape1 = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");
	auto shape2 = CreateShape("shape2", gfx::Color::FromHexStr("000000"), "rectangle", "200 250 30 40");
	auto shape3 = CreateShape("shape3", gfx::Color::FromHexStr("aaaaaa"), "line", "0 0 100 100");

	picture.AddShape(std::move(shape1));
	picture.AddShape(std::move(shape3));
	picture.AddShape(std::move(shape2));

	auto shapesList = picture.ListShapes();
	EXPECT_EQ(3, shapesList.size());
	EXPECT_EQ("shape1", shapesList[0]->GetId());
	EXPECT_EQ("shape3", shapesList[1]->GetId());
	EXPECT_EQ("shape2", shapesList[2]->GetId());
}

TEST_F(PictureTests, PictureCanDrawShape)
{
	shapes::Picture picture;
	auto shape = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");
	picture.AddShape(std::move(shape));

	gfx::MockCanvas canvas;
	EXPECT_NO_THROW(picture.DrawShape("shape1", canvas));

	EXPECT_GT(canvas.GetCallCount(), 0);
}

TEST_F(PictureTests, CannotDrawNonExistentShape)
{
	shapes::Picture picture;
	gfx::MockCanvas canvas;

	EXPECT_THROW(picture.DrawShape("nonexistent", canvas), std::runtime_error);
}

TEST_F(PictureTests, CanDrawPicture)
{
	shapes::Picture picture;
	auto shape1 = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");
	auto shape2 = CreateShape("shape2", gfx::Color::FromHexStr("000000"), "rectangle", "200 250 30 40");
	picture.AddShape(std::move(shape1));
	picture.AddShape(std::move(shape2));
	gfx::MockCanvas canvas;

	EXPECT_NO_THROW(picture.DrawPicture(canvas));

	EXPECT_GT(canvas.GetCallCount(), 0);
}

TEST_F(PictureTests, PictureCanChangeShapeColor)
{
	shapes::Picture picture;
	auto shape = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");
	picture.AddShape(std::move(shape));

	EXPECT_EQ("circle shape1 #ffffff 100 150 25", picture.ListShapes()[0]->GetDescription());

	gfx::Color newColor = gfx::Color::FromHexStr("333aaa");
	picture.ChangeShapeColor("shape1", newColor);

	EXPECT_EQ("circle shape1 #333aaa 100 150 25", picture.ListShapes()[0]->GetDescription());
}

TEST_F(PictureTests, PictureCanChangeShapeStrategy)
{
	shapes::Picture picture;
	auto shape = CreateShape("shape1", gfx::Color::FromHexStr("ffffff"), "circle", "100 150 25");
	picture.AddShape(std::move(shape));

	EXPECT_EQ("circle shape1 #ffffff 100 150 25", picture.ListShapes()[0]->GetDescription());

	auto newStrategy = CreateStrategy("rectangle", "150 100 20 30");
	picture.ChangeShapeStrategy("shape1", std::move(newStrategy));

	EXPECT_EQ("rectangle shape1 #ffffff 150 100 20 30", picture.ListShapes()[0]->GetDescription());
}

TEST_F(PictureTests, PictureCannotChangeColorOfNonExistentShape)
{
	shapes::Picture picture;
	gfx::Color newColor = gfx::Color::FromHexStr("333aaa");

	EXPECT_THROW(picture.ChangeShapeColor("nonexistent", newColor), std::runtime_error);
}

TEST_F(PictureTests, PictureCannotChangeStrategyOfNonExistentShape)
{
	shapes::Picture picture;
	auto newStrategy = CreateStrategy("rectangle", "150 100 20 30");

	EXPECT_THROW(picture.ChangeShapeStrategy("nonexistent", std::move(newStrategy)), std::runtime_error);
}