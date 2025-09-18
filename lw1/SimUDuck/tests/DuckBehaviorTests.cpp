#include "../lib/Duck/Fly/FlyNoWay.h"
#include "MockDanceBehavior.h"
#include "MockDuck.h"

#include <gtest/gtest.h>
#include <memory>

class DuckBehaviorTest : public testing::Test {
protected:
    MockDanceBehavior *mockDancePtr = nullptr;
    std::unique_ptr<MockDuck> duck;

    void SetUp() override {
        auto mockDance = std::make_unique<MockDanceBehavior>();

        mockDancePtr = mockDance.get();

        duck = std::make_unique<MockDuck>(std::move(mockDance));
    }
};

TEST_F(DuckBehaviorTest, DanceCallsDanceBehavior) {
    EXPECT_EQ(mockDancePtr->GetCallCount(), 0);

    duck->Dance();

    EXPECT_TRUE(mockDancePtr->GetCallCount(), 1);
}
