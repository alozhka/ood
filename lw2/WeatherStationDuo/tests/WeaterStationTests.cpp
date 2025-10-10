#include "../src/Display.h"
#include "BrokenDisplay.h"
#include "MockDisplay.h"
#include "OrderTestDisplay.h"

#include "gtest/gtest.h"

class WeatherStationTests : public testing::Test
{
protected:
	WeatherData indoor{};
	OutWeatherData outdoor{};
};

TEST_F(WeatherStationTests, ObserverSafelyUnsubscibesItself)
{
	BrokenDisplay display{ indoor };
	indoor.RegisterObserver(display, 1);

	indoor.SetMeasurements(1, 1, 1);
	indoor.SetMeasurements(1, 1, 1);

	EXPECT_EQ(1, display.GetCallCount());
}

TEST_F(WeatherStationTests, ObserversAreNotifiedByPriority)
{
	std::vector<int> callOrder;

	OrderTestDisplay display1(callOrder, 1);
	OrderTestDisplay display2(callOrder, 2);
	OrderTestDisplay display3(callOrder, 3);
	OrderTestDisplay display4(callOrder, 4);

	indoor.RegisterObserver(display1, 5);
	indoor.RegisterObserver(display3, 10);
	indoor.RegisterObserver(display2, 10);
	indoor.RegisterObserver(display4, 3);

	indoor.SetMeasurements(1, 1, 1);

	ASSERT_EQ(4u, callOrder.size());
	EXPECT_EQ(callOrder[0], 3);
	EXPECT_EQ(callOrder[1], 2);
	EXPECT_EQ(callOrder[2], 1);
	EXPECT_EQ(callOrder[3], 4);
}

TEST_F(WeatherStationTests, DuplicateRegistrationIsIgnored)
{
	WeatherData data;
	std::vector<int> callOrder;

	OrderTestDisplay display(callOrder, 1);

	data.RegisterObserver(display, 5);
	data.RegisterObserver(display, 10);

	data.SetMeasurements(1, 1, 1);

	ASSERT_EQ(1u, callOrder.size());
}

TEST_F(WeatherStationTests, RemovingNonExistentObserverIsIgnored)
{
	BrokenDisplay display(indoor);

	indoor.RemoveObserver(display);

	EXPECT_NO_THROW(indoor.RemoveObserver(display));
}

TEST_F(WeatherStationTests, DisplayGetsDataFromBothSensors)
{
	MockDisplay display(indoor, outdoor);

	indoor.SetMeasurements(1, 1, 1);

	EXPECT_EQ(1, display.GetCallCount());

	outdoor.SetMeasurements(1, 1, 1, 10, 30);

	EXPECT_EQ(2, display.GetCallCount());
}