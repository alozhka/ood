#include "BrokenDisplay.h"

#include "gtest/gtest.h"

class WeatherStationTests : public testing::Test
{
};

TEST(WeatherStationTests, ObserverSafelyUnsubsc)
{
	std::ostringstream out;
	WeatherData data;
	BrokenDisplay display(data, out);

	data.SetMeasurements(1, 1, 1);
	data.SetMeasurements(1, 1, 1);

	EXPECT_EQ("temp: 1, humidity: 1, pressure: 1\n", out.str());
}