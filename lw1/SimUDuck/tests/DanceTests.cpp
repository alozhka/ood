#include "MockDanceBehavior.h"
#include "TestDuck.h"
#include <gtest/gtest.h>
#include <memory>

class DuckDanceTest : public testing::Test
{
};

TEST_F(DuckDanceTest, DanceCallsDanceBehavior)
{
	// Arrange
	auto mockDance = std::make_unique<MockDanceBehavior>();
	MockDanceBehavior* mockPtr = mockDance.get();
	TestDuck duck(std::move(mockDance));

	// Act
	duck.Dance();

	// Assert
	EXPECT_TRUE(mockPtr->WasCalled());
}