#include "../src/adapter/ModernGraphicsAdapter.h"
#include "../src/adapter/ModernGraphicsClassAdapter.h"
#include "../src/modern_graphics_lib.h"
#include "../src/shapes_drawing_lib.h"
#include <gtest/gtest.h>
#include <sstream>

class AdapterTests : public testing::Test
{
protected:
	shape_drawing_lib::Triangle triangle{ { 10, 15 }, { 100, 200 }, { 150, 250 } };
	shape_drawing_lib::Rectangle rectangle{ { 30, 40 }, 18, 24 };
};

TEST_F(AdapterTests, ObjectAdapterDrawsPictures)
{
	std::ostringstream output;
	modern_graphics_lib::ModernGraphicsRenderer renderer(output);
	ModernGraphicsAdapter adapter(renderer);
	shape_drawing_lib::CanvasPainter painter(adapter);

	renderer.BeginDraw();
	painter.Draw(triangle);
	painter.Draw(rectangle);
	renderer.EndDraw();

	std::string expected = R"(<draw>
  <line fromX="10" fromY="15" toX="100" toY="200"/>
  <line fromX="100" fromY="200" toX="150" toY="250"/>
  <line fromX="150" fromY="250" toX="10" toY="15"/>
  <line fromX="30" fromY="40" toX="48" toY="40"/>
  <line fromX="48" fromY="40" toX="48" toY="64"/>
  <line fromX="48" fromY="64" toX="30" toY="64"/>
  <line fromX="30" fromY="64" toX="30" toY="40"/>
</draw>
)";
	EXPECT_EQ(expected, output.str());
}

TEST_F(AdapterTests, ClassAdapterDrawsPictures)
{
	std::ostringstream output;
	{
		ModernGraphicsClassAdapter adapter(output);
		shape_drawing_lib::CanvasPainter painter(adapter);
		painter.Draw(triangle);
		painter.Draw(rectangle);
	}

	std::string expected = R"(<draw>
  <line fromX="10" fromY="15" toX="100" toY="200"/>
  <line fromX="100" fromY="200" toX="150" toY="250"/>
  <line fromX="150" fromY="250" toX="10" toY="15"/>
  <line fromX="30" fromY="40" toX="48" toY="40"/>
  <line fromX="48" fromY="40" toX="48" toY="64"/>
  <line fromX="48" fromY="64" toX="30" toY="64"/>
  <line fromX="30" fromY="64" toX="30" toY="40"/>
</draw>
)";
	EXPECT_EQ(expected, output.str());
}