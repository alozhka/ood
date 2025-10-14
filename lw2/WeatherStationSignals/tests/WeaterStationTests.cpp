#include "../src/WeatherData.h"
#include "gtest/gtest.h"
#include <vector>

class WeatherStationTests : public testing::Test
{
protected:
	WeatherData weatherData{};
};

TEST_F(WeatherStationTests, ObserverReceivesOnlySubscribedEvent)
{
	int callCount = 0;
	Connection conn1 = weatherData.DoOnTemperatureChanged([&callCount](const WeatherInfo&) { ++callCount; });
	Connection conn2 = weatherData.DoOnPressureChanged([&callCount](const WeatherInfo&) { ++callCount; });

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, callCount);

	weatherData.SetPressure(760.0);
	EXPECT_EQ(2, callCount);

	weatherData.SetHumidity(60.0);
	EXPECT_EQ(2, callCount);
}

TEST_F(WeatherStationTests, ObserversNotifiedByPriority)
{
	std::vector<int> callOrder;
	Connection conn1 = weatherData.DoOnTemperatureChanged(
		[&callOrder](const WeatherInfo&) { callOrder.push_back(1); },
		5);
	Connection conn2 = weatherData.DoOnTemperatureChanged(
		[&callOrder](const WeatherInfo&) { callOrder.push_back(2); },
		10);
	Connection conn3 = weatherData.DoOnTemperatureChanged(
		[&callOrder](const WeatherInfo&) { callOrder.push_back(3); },
		3);

	weatherData.SetTemperature(25.0);

	ASSERT_EQ(3u, callOrder.size());
	EXPECT_EQ(2, callOrder[0]); // Приоритет 10
	EXPECT_EQ(1, callOrder[1]); // Приоритет 5
	EXPECT_EQ(3, callOrder[2]); // Приоритет 3
}

TEST_F(WeatherStationTests, MultipleSubscribersToSameEvent)
{
	int subscriber1CallCount = 0;
	int subscriber2CallCount = 0;
	int subscriber3CallCount = 0;
	Connection conn1 = weatherData.DoOnTemperatureChanged([&subscriber1CallCount](const WeatherInfo&) { ++subscriber1CallCount; });
	Connection conn2 = weatherData.DoOnTemperatureChanged([&subscriber2CallCount](const WeatherInfo&) { ++subscriber2CallCount; });
	Connection conn3 = weatherData.DoOnTemperatureChanged([&subscriber3CallCount](const WeatherInfo&) { ++subscriber3CallCount; });

	weatherData.SetTemperature(25.0);

	EXPECT_EQ(1, subscriber1CallCount);
	EXPECT_EQ(1, subscriber2CallCount);
	EXPECT_EQ(1, subscriber3CallCount);
}

TEST_F(WeatherStationTests, SetMeasurementsTriggersAllSignals)
{
	int temperatureCallCount = 0;
	int humidityCallCount = 0;
	int pressureCallCount = 0;
	int windSpeedCallCount = 0;
	int windDirectionCallCount = 0;

	weatherData.DoOnTemperatureChanged([&temperatureCallCount](const WeatherInfo&) { ++temperatureCallCount; });
	weatherData.DoOnHumidityChanged([&humidityCallCount](const WeatherInfo&) { ++humidityCallCount; });
	weatherData.DoOnPressureChanged([&pressureCallCount](const WeatherInfo&) { ++pressureCallCount; });
	weatherData.DoOnWindSpeedChanged([&windSpeedCallCount](const WeatherInfo&) { ++windSpeedCallCount; });
	weatherData.DoOnWindDirectionChanged([&windDirectionCallCount](const WeatherInfo&) { ++windDirectionCallCount; });

	weatherData.SetMeasurements(25.0, 60.0, 760.0, 15.0, 180.0);

	EXPECT_EQ(1, temperatureCallCount);
	EXPECT_EQ(1, humidityCallCount);
	EXPECT_EQ(1, pressureCallCount);
	EXPECT_EQ(1, windSpeedCallCount);
	EXPECT_EQ(1, windDirectionCallCount);
}

TEST_F(WeatherStationTests, CorrectDataIsPassedToSubscribers)
{
	WeatherInfo receivedData;
	weatherData.DoOnTemperatureChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
	weatherData.DoOnHumidityChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
	weatherData.DoOnPressureChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
	weatherData.DoOnWindSpeedChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
	weatherData.DoOnWindDirectionChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });

	weatherData.SetMeasurements(25.5, 65.3, 755.2, 12.7, 270.5);

	EXPECT_DOUBLE_EQ(25.5, receivedData.temperature);
	EXPECT_DOUBLE_EQ(65.3, receivedData.humidity);
	EXPECT_DOUBLE_EQ(755.2, receivedData.pressure);
	EXPECT_DOUBLE_EQ(12.7, receivedData.windSpeed);
	EXPECT_DOUBLE_EQ(270.5, receivedData.windDirection);
}

TEST_F(WeatherStationTests, DisconnectOneSubscriberDoesNotAffectOthers)
{
	int subscriber1CallCount = 0;
	int subscriber2CallCount = 0;
	Connection conn1 = weatherData.DoOnTemperatureChanged([&subscriber1CallCount](const WeatherInfo&) { ++subscriber1CallCount; });
	Connection conn2 = weatherData.DoOnTemperatureChanged([&subscriber2CallCount](const WeatherInfo&) { ++subscriber2CallCount; });

	weatherData.SetTemperature(25.0);

	EXPECT_EQ(1, subscriber1CallCount);
	EXPECT_EQ(1, subscriber2CallCount);

	conn1.disconnect();

	weatherData.SetTemperature(30.0);
	EXPECT_EQ(1, subscriber1CallCount);
	EXPECT_EQ(2, subscriber2CallCount);
}