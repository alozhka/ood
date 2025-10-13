#include "../src/WeatherData.h"
#include "gtest/gtest.h"
#include <vector>

class WeatherStationTests : public testing::Test
{
protected:
	WeatherData weatherData{};
};

// Тест: подписчик получает только те события, на которые подписан
TEST_F(WeatherStationTests, ObserverReceivesOnlySubscribedEvent)
{
	int temperatureCallCount = 0;
	int humidityCallCount = 0;

	// Подписываемся только на температуру
	auto conn = weatherData.DoOnTemperatureChanged(
		[&temperatureCallCount](const WeatherInfo& data) {
			++temperatureCallCount;
			EXPECT_EQ(-20, data.temperature);
		});

	weatherData.DoOnHumidityChanged(
		[&humidityCallCount](const WeatherInfo& data) {
			++humidityCallCount;
		});

	weatherData.SetTemperature(-20);
	EXPECT_EQ(1, temperatureCallCount);

	weatherData.SetHumidity(60.0);
	EXPECT_EQ(1, temperatureCallCount); // Не должен увеличиться
	EXPECT_EQ(1, humidityCallCount);
}

// Тест: подписчик может подписаться на несколько событий
TEST_F(WeatherStationTests, ObserverCanSubscribeToMultipleEvents)
{
	int callCount = 0;

	auto conn1 = weatherData.DoOnTemperatureChanged(
		[&callCount](const WeatherInfo& data) { ++callCount; });

	auto conn2 = weatherData.DoOnPressureChanged(
		[&callCount](const WeatherInfo& data) { ++callCount; });

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, callCount);

	weatherData.SetPressure(760.0);
	EXPECT_EQ(2, callCount);

	weatherData.SetHumidity(60.0);
	EXPECT_EQ(2, callCount); // Не изменилось, т.к. не подписаны на влажность
}

// Тест: подписчики уведомляются по приоритету
TEST_F(WeatherStationTests, ObserversNotifiedByPriority)
{
	std::vector<int> callOrder;

	// Подписываем с разными приоритетами
	auto conn1 = weatherData.DoOnTemperatureChanged(
		[&callOrder](const WeatherInfo& data) { callOrder.push_back(1); }, 5);

	auto conn2 = weatherData.DoOnTemperatureChanged(
		[&callOrder](const WeatherInfo& data) { callOrder.push_back(2); }, 10);

	auto conn3 = weatherData.DoOnTemperatureChanged(
		[&callOrder](const WeatherInfo& data) { callOrder.push_back(3); }, 3);

	weatherData.SetTemperature(25.0);

	ASSERT_EQ(3u, callOrder.size());
	EXPECT_EQ(2, callOrder[0]); // Приоритет 10
	EXPECT_EQ(1, callOrder[1]); // Приоритет 5
	EXPECT_EQ(3, callOrder[2]); // Приоритет 3
}

// Тест: отписка работает корректно
TEST_F(WeatherStationTests, UnsubscribeWorks)
{
	int callCount = 0;

	auto conn = weatherData.DoOnTemperatureChanged(
		[&callCount](const WeatherInfo& data) { ++callCount; });

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, callCount);

	// Отписываемся
	conn.disconnect();

	weatherData.SetTemperature(30.0);
	EXPECT_EQ(1, callCount); // Не должен увеличиться
}

// Тест: несколько подписчиков на одно событие
TEST_F(WeatherStationTests, MultipleSubscribersToSameEvent)
{
	int subscriber1CallCount = 0;
	int subscriber2CallCount = 0;
	int subscriber3CallCount = 0;

	auto conn1 = weatherData.DoOnTemperatureChanged(
		[&subscriber1CallCount](const WeatherInfo& data) { ++subscriber1CallCount; });

	auto conn2 = weatherData.DoOnTemperatureChanged(
		[&subscriber2CallCount](const WeatherInfo& data) { ++subscriber2CallCount; });

	auto conn3 = weatherData.DoOnTemperatureChanged(
		[&subscriber3CallCount](const WeatherInfo& data) { ++subscriber3CallCount; });

	weatherData.SetTemperature(25.0);

	EXPECT_EQ(1, subscriber1CallCount);
	EXPECT_EQ(1, subscriber2CallCount);
	EXPECT_EQ(1, subscriber3CallCount);
}

// Тест: SetMeasurements вызывает все соответствующие сигналы
TEST_F(WeatherStationTests, SetMeasurementsTriggersAllSignals)
{
	int temperatureCallCount = 0;
	int humidityCallCount = 0;
	int pressureCallCount = 0;
	int windSpeedCallCount = 0;
	int windDirectionCallCount = 0;

	weatherData.DoOnTemperatureChanged(
		[&temperatureCallCount](const WeatherInfo& data) { ++temperatureCallCount; });

	weatherData.DoOnHumidityChanged(
		[&humidityCallCount](const WeatherInfo& data) { ++humidityCallCount; });

	weatherData.DoOnPressureChanged(
		[&pressureCallCount](const WeatherInfo& data) { ++pressureCallCount; });

	weatherData.DoOnWindSpeedChanged(
		[&windSpeedCallCount](const WeatherInfo& data) { ++windSpeedCallCount; });

	weatherData.DoOnWindDirectionChanged(
		[&windDirectionCallCount](const WeatherInfo& data) { ++windDirectionCallCount; });

	weatherData.SetMeasurements(25.0, 60.0, 760.0, 15.0, 180.0);

	EXPECT_EQ(1, temperatureCallCount);
	EXPECT_EQ(1, humidityCallCount);
	EXPECT_EQ(1, pressureCallCount);
	EXPECT_EQ(1, windSpeedCallCount);
	EXPECT_EQ(1, windDirectionCallCount);
}

// Тест: данные корректно передаются подписчикам
TEST_F(WeatherStationTests, CorrectDataIsPassedToSubscribers)
{
	WeatherInfo receivedData;

	weatherData.DoOnTemperatureChanged(
		[&receivedData](const WeatherInfo& data) { receivedData = data; });

	weatherData.SetMeasurements(25.5, 65.3, 755.2, 12.7, 270.5);

	EXPECT_DOUBLE_EQ(25.5, receivedData.temperature);
	EXPECT_DOUBLE_EQ(65.3, receivedData.humidity);
	EXPECT_DOUBLE_EQ(755.2, receivedData.pressure);
	EXPECT_DOUBLE_EQ(12.7, receivedData.windSpeed);
	EXPECT_DOUBLE_EQ(270.5, receivedData.windDirection);
}

// Тест: отписка одного подписчика не влияет на других
TEST_F(WeatherStationTests, DisconnectOneSubscriberDoesNotAffectOthers)
{
	int subscriber1CallCount = 0;
	int subscriber2CallCount = 0;

	auto conn1 = weatherData.DoOnTemperatureChanged(
		[&subscriber1CallCount](const WeatherInfo& data) { ++subscriber1CallCount; });

	auto conn2 = weatherData.DoOnTemperatureChanged(
		[&subscriber2CallCount](const WeatherInfo& data) { ++subscriber2CallCount; });

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, subscriber1CallCount);
	EXPECT_EQ(1, subscriber2CallCount);

	// Отписываем первого подписчика
	conn1.disconnect();

	weatherData.SetTemperature(30.0);
	EXPECT_EQ(1, subscriber1CallCount); // Не изменилось
	EXPECT_EQ(2, subscriber2CallCount); // Увеличилось
}