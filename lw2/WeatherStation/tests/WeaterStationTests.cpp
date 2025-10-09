#include "../src/Display.h"
#include "BrokenDisplay.h"
#include "OrderTestDisplay.h"

#include "gtest/gtest.h"

class WeatherStationTests : public testing::Test
{
};

TEST(WeatherStationTests, ObserverSafelyUnsubscibesItself)
{
	std::ostringstream out;
	WeatherData data;
	BrokenDisplay display(data, out);

	data.SetMeasurements(1, 1, 1);
	data.SetMeasurements(1, 1, 1);

	EXPECT_EQ("temp: 1, humidity: 1, pressure: 1\n", out.str());
}

TEST(WeatherStationTests, ObserversAreNotifiedByPriority)
{
	WeatherData data;
	std::vector<int> callOrder;

	OrderTestDisplay display1(callOrder, 1);
	OrderTestDisplay display2(callOrder, 2);
	OrderTestDisplay display3(callOrder, 3);
	OrderTestDisplay display4(callOrder, 4);

	data.RegisterObserver(display1, 5);
	data.RegisterObserver(display3, 10);
	data.RegisterObserver(display2, 10);
	data.RegisterObserver(display4, 3);

	data.SetMeasurements(1, 1, 1);

	ASSERT_EQ(4u, callOrder.size());
	EXPECT_EQ(callOrder[0], 3);
	EXPECT_EQ(callOrder[1], 2);
	EXPECT_EQ(callOrder[2], 1);
	EXPECT_EQ(callOrder[3], 4);
}

TEST(WeatherStationTests, DuplicateRegistrationIsIgnored)
{
	WeatherData data;
	std::vector<int> callOrder;

	OrderTestDisplay display(callOrder, 1);

	data.RegisterObserver(display, 5);
	data.RegisterObserver(display, 10);

	data.SetMeasurements(1, 1, 1);

	ASSERT_EQ(1u, callOrder.size());
}

TEST(WeatherStationTests, RemovingNonExistentObserverIsIgnored)
{
	WeatherData data;
	Display display;

	EXPECT_NO_THROW(data.RemoveObserver(display));
}
