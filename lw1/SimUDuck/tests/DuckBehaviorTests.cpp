#include "MockDanceBehavior.h"
#include "MockDuck.h"
#include "MockFlyBehavior.h"
#include "MockQuackBehavior.h"
#include "../lib/Duck/Dance/NoDanceBehavior.h"
#include "../lib/Duck/Fly/FlyNoWay.h"

#include <gtest/gtest.h>
#include <memory>


class DuckBehaviorTest : public testing::Test
{
protected:
	void SetUp() override
	{
		auto mockQuack = std::make_unique<MockQuackBehavior>();
		auto mockFly = std::make_unique<MockFlyBehavior>();
		auto mockDance = std::make_unique<MockDanceBehavior>();

		mockDancePtr = mockDance.get();
		mockFlyPtr = mockFly.get();
		mockQuackPtr = mockQuack.get();

		duck = std::make_unique<MockDuck>(
			std::move(mockQuack),
			std::move(mockFly),
			std::move(mockDance));
	}

	MockQuackBehavior* mockQuackPtr = nullptr;
	MockFlyBehavior* mockFlyPtr = nullptr;
	MockDanceBehavior* mockDancePtr = nullptr;
	std::unique_ptr<MockDuck> duck;
};

TEST_F(DuckBehaviorTest, DanceCallsDanceBehavior)
{
	EXPECT_EQ(0, mockDancePtr->GetCallCount());

	duck->Dance();

	EXPECT_EQ(1, mockDancePtr->GetCallCount());
}

TEST_F(DuckBehaviorTest, QuackCallsQuackBehavior)
{
	EXPECT_EQ(0, mockQuackPtr->GetQuackCount());

	duck->Quack();

	EXPECT_EQ(1, mockQuackPtr->GetQuackCount());
}

TEST_F(DuckBehaviorTest, FlyCallsFlyBehavior)
{
	EXPECT_EQ(0, mockFlyPtr->GetFlyCount());

	duck->Fly();

	EXPECT_EQ(1, mockFlyPtr->GetFlyCount());
}

TEST_F(DuckBehaviorTest, FlyableDucksQuackEverySecondFly)
{
	duck->Fly();

	EXPECT_EQ(1, mockFlyPtr->GetFlyCount());
	EXPECT_EQ(0, mockQuackPtr->GetQuackCount());

	duck->Fly();

	EXPECT_EQ(2, mockFlyPtr->GetFlyCount());
	EXPECT_EQ(1, mockQuackPtr->GetQuackCount());
}

TEST_F(DuckBehaviorTest, NonFlyingDuckDoesNotQuack)
{
	auto mockQuack = std::make_unique<MockQuackBehavior>();
	MockQuackBehavior* mockQuackPtr = mockQuack.get();
	auto duck = MockDuck(
		std::move(mockQuack),
		std::make_unique<FlyNoWay>(),
		std::make_unique<NoDanceBehavior>());

	duck.Fly();
	duck.Fly();

	EXPECT_EQ(0, mockQuackPtr->GetQuackCount());
}