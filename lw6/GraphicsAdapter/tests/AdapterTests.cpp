#include "../src/adapter/ModernGraphicsAdapter.h"
#include "../src/adapter/ModernGraphicsClassAdapter.h"
#include "../src/modern_graphics_lib.h"
#include "../src/shapes_drawing_lib.h"
#include <gtest/gtest.h>
#include <sstream>

class AdapterTests : public testing::Test
{
protected:
	shape_drawing_lib::Triangle triangle{ { 10, 15 }, { 100, 200 }, { 150, 250 }, 0xFFFF00 };
	shape_drawing_lib::Rectangle rectangle{ { 30, 40 }, 18, 24, 0x800000FF };
};

TEST_F(AdapterTests, ObjectAdapterDrawsPictures)
{
	std::ostringstream output;
	modern_graphics_lib::ModernGraphicsRenderer renderer(output);
	ModernGraphicsAdapter objectAdapter(renderer);
	shape_drawing_lib::CanvasPainter painter(objectAdapter);

	renderer.BeginDraw();
	painter.Draw(triangle);
	painter.Draw(rectangle);
	renderer.EndDraw();

	std::string expected = R"(<draw>
<line fromX="10" fromY="15" toX="100" toY="200"><color r="1" g="1" b="0" a="1" /></line>
<line fromX="100" fromY="200" toX="150" toY="250"><color r="1" g="1" b="0" a="1" /></line>
<line fromX="150" fromY="250" toX="10" toY="15"><color r="1" g="1" b="0" a="1" /></line>
<line fromX="30" fromY="40" toX="48" toY="40"><color r="0" g="0" b="1" a="0.5019608" /></line>
<line fromX="48" fromY="40" toX="48" toY="64"><color r="0" g="0" b="1" a="0.5019608" /></line>
<line fromX="48" fromY="64" toX="30" toY="64"><color r="0" g="0" b="1" a="0.5019608" /></line>
<line fromX="30" fromY="64" toX="30" toY="40"><color r="0" g="0" b="1" a="0.5019608" /></line>
</draw>
)";
	EXPECT_EQ(expected, output.str());
}

TEST_F(AdapterTests, ClassAdapterDrawsPictures)
{
	std::ostringstream output;
	ModernGraphicsClassAdapter renderer(output);
	shape_drawing_lib::CanvasPainter painter(renderer);

	renderer.BeginDraw();
	painter.Draw(triangle);
	painter.Draw(rectangle);
	renderer.EndDraw();

	std::string expected = R"(<draw>
<line fromX="10" fromY="15" toX="100" toY="200"><color r="1" g="1" b="0" a="1" /></line>
<line fromX="100" fromY="200" toX="150" toY="250"><color r="1" g="1" b="0" a="1" /></line>
<line fromX="150" fromY="250" toX="10" toY="15"><color r="1" g="1" b="0" a="1" /></line>
<line fromX="30" fromY="40" toX="48" toY="40"><color r="0" g="0" b="1" a="0.5019608" /></line>
<line fromX="48" fromY="40" toX="48" toY="64"><color r="0" g="0" b="1" a="0.5019608" /></line>
<line fromX="48" fromY="64" toX="30" toY="64"><color r="0" g="0" b="1" a="0.5019608" /></line>
<line fromX="30" fromY="64" toX="30" toY="40"><color r="0" g="0" b="1" a="0.5019608" /></line>
</draw>
)";
	EXPECT_EQ(expected, output.str());
}