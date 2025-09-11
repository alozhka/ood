#include "MockFlyBehavior.h"
#include "MockQuackBehavior.h"
#include "TestDuckWithMocks.h"
#include "../lib/Duck/Fly/FlyNoWay.h"
#include <gtest/gtest.h>
#include <memory>

class FlightPleasureTest : public testing::Test
{
protected:
	void SetUp() override
	{
		mockFly = std::make_unique<MockFlyBehavior>();
		mockQuack = std::make_unique<MockQuackBehavior>();
		
		// Сохраняем указатели для проверки состояния
		mockFlyPtr = mockFly.get();
		mockQuackPtr = mockQuack.get();
		
		duck = std::make_unique<TestDuckWithMocks>(
			std::move(mockFly), 
			std::move(mockQuack)
		);
	}

	std::unique_ptr<MockFlyBehavior> mockFly;
	std::unique_ptr<MockQuackBehavior> mockQuack;
	MockFlyBehavior* mockFlyPtr;
	MockQuackBehavior* mockQuackPtr;
	std::unique_ptr<TestDuckWithMocks> duck;
};

TEST_F(FlightPleasureTest, FirstFlightNoQuack)
{
	// Arrange & Act
	duck->FlyWithPleasure();

	// Assert
	EXPECT_TRUE(mockFlyPtr->WasFlyWithCallbackCalled());
	EXPECT_EQ(1, mockFlyPtr->GetFlightCount());
	EXPECT_FALSE(mockFlyPtr->WasCallbackInvoked());
	EXPECT_EQ(0, mockQuackPtr->GetQuackCount());
}

TEST_F(FlightPleasureTest, SecondFlightWithQuack)
{
	// Arrange
	duck->FlyWithPleasure(); // Первый полёт
	
	// Act
	duck->FlyWithPleasure(); // Второй полёт

	// Assert
	EXPECT_EQ(2, mockFlyPtr->GetFlightCount());
	EXPECT_TRUE(mockFlyPtr->WasCallbackInvoked());
	EXPECT_EQ(1, mockQuackPtr->GetQuackCount());
}

TEST_F(FlightPleasureTest, RegularFlyDoesNotTriggerQuack)
{
	// Arrange & Act
	duck->Fly(); // Обычный полёт без коллбека
	duck->Fly(); // Второй обычный полёт

	// Assert
	EXPECT_TRUE(mockFlyPtr->WasFlyCalled());
	EXPECT_FALSE(mockFlyPtr->WasFlyWithCallbackCalled());
	EXPECT_EQ(0, mockQuackPtr->GetQuackCount());
}

// Тест для нелетающих уток
class NonFlyingDuckTest : public testing::Test
{
protected:
	void SetUp() override
	{
		mockQuack = std::make_unique<MockQuackBehavior>();
		mockQuackPtr = mockQuack.get();
		
		// Используем FlyNoWay для нелетающей утки
		duck = std::make_unique<TestDuckWithMocks>(
			std::make_unique<FlyNoWay>(),
			std::move(mockQuack)
		);
	}

	std::unique_ptr<MockQuackBehavior> mockQuack;
	MockQuackBehavior* mockQuackPtr;
	std::unique_ptr<TestDuckWithMocks> duck;
};

TEST_F(NonFlyingDuckTest, NonFlyingDuckDoesNotQuack)
{
	// Act
	duck->FlyWithPleasure(); // Первый "полёт"
	duck->FlyWithPleasure(); // Второй "полёт"

	// Assert
	EXPECT_EQ(0, mockQuackPtr->GetQuackCount()); // Нелетающие утки не крякают от удовольствия
}