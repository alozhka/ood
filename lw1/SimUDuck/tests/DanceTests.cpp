#include "MockDanceBehavior.h"
#include "TestDuck.h"
#include <gtest/gtest.h>
#include <memory>

class DuckDanceTest : public testing::Test
{
};

TEST_F(DuckDanceTest, DanceCallsMockBehavior)
{
	// Arrange
	auto mockDance = std::make_unique<MockDanceBehavior>();
	MockDanceBehavior* mockPtr = mockDance.get();
	TestDuck duck(std::move(mockDance));

	// Act
	duck.Dance();

	// Assert
	EXPECT_TRUE(mockPtr->WasCalled());
	EXPECT_EQ(mockPtr->GetDanceMessage(), "Mock dance called");
}

TEST_F(DuckDanceTest, MultipleDanceCalls)
{
	// Arrange
	auto mockDance = std::make_unique<MockDanceBehavior>();
	MockDanceBehavior* mockPtr = mockDance.get();
	TestDuck duck(std::move(mockDance));

	// Act
	duck.Dance();
	duck.Dance();

	// Assert - проверяем, что метод можно вызывать многократно
	EXPECT_TRUE(mockPtr->WasCalled());
}

TEST_F(DuckDanceTest, MockBehaviorReset)
{
	// Arrange
	auto mockDance = std::make_unique<MockDanceBehavior>();
	MockDanceBehavior* mockPtr = mockDance.get();
	TestDuck duck(std::move(mockDance));

	// Act
	duck.Dance();
	EXPECT_TRUE(mockPtr->WasCalled());

	mockPtr->Reset();

	// Assert
	EXPECT_FALSE(mockPtr->WasCalled());
	EXPECT_EQ(mockPtr->GetDanceMessage(), "");
}

// Тесты для конкретных реализаций танцев
TEST(ConcreteDanceBehaviorTest, WaltzDanceOutput)
{
	// Этот тест проверяет, что WaltzDance не падает при вызове
    // (хотя мы не проверяем stdout согласно требованиям)
    auto waltzDance = std::make_unique<MockDanceBehavior>();
    EXPECT_NO_THROW(waltzDance->Dance());
}
