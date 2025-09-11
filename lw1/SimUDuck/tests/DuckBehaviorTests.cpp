#include "../lib/Duck/Dance/NoDanceBehavior.h"
#include "../lib/Duck/Fly/FlyNoWay.h"
#include "MockDanceBehavior.h"
#include "MockFlyBehavior.h"
#include "MockQuackBehavior.h"
#include "TestDuckWithMocks.h"

#include <gtest/gtest.h>
#include <memory>

class DuckBehaviorTest : public testing::Test
{
protected:
	MockFlyBehavior* mockFlyPtr = nullptr;
	MockQuackBehavior* mockQuackPtr = nullptr;
	MockDanceBehavior* mockDancePtr = nullptr;
	std::unique_ptr<TestDuckWithMocks> duck;

	void SetUp() override
	{
		auto mockFly = std::make_unique<MockFlyBehavior>();
		auto mockQuack = std::make_unique<MockQuackBehavior>();
		auto mockDance = std::make_unique<MockDanceBehavior>();

		mockFlyPtr = mockFly.get();
		mockQuackPtr = mockQuack.get();
		mockDancePtr = mockDance.get();

		duck = std::make_unique<TestDuckWithMocks>(
			std::move(mockFly),
			std::move(mockQuack),
			std::move(mockDance));
	}
};

TEST_F(DuckBehaviorTest, DanceCallsDanceBehavior)
{
	duck->Dance();

	EXPECT_TRUE(mockDancePtr->WasCalled());
}

TEST_F(DuckBehaviorTest, FlyableDucksQuackWhileFlying)
{
	duck->Fly();

	EXPECT_TRUE(mockFlyPtr->WasFlyWithCallbackCalled());
	EXPECT_EQ(1, mockFlyPtr->GetFlightCount());
	EXPECT_FALSE(mockFlyPtr->WasCallbackInvoked());
	EXPECT_EQ(0, mockQuackPtr->GetQuackCount());

	duck->Fly();

	EXPECT_EQ(2, mockFlyPtr->GetFlightCount());
	EXPECT_TRUE(mockFlyPtr->WasCallbackInvoked());
	EXPECT_EQ(1, mockQuackPtr->GetQuackCount());
}

TEST_F(DuckBehaviorTest, NonFlyingDuckDoesNotQuack)
{
	auto mockQuack = std::make_unique<MockQuackBehavior>();
	MockQuackBehavior* mockQuackBehaviorPtr = mockQuack.get();
	auto duck = TestDuckWithMocks(
		std::make_unique<FlyNoWay>(),
		std::move(mockQuack),
		std::make_unique<NoDanceBehavior>());

	duck.Fly();
	duck.Fly();

	EXPECT_EQ(0, mockQuackBehaviorPtr->GetQuackCount());
}
