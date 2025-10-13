#include "src/Display.h"
#include "src/WeatherData.h"
#include <iostream>

void TestWeatherStation()
{
	std::cout << "=== Weather Station Test with boost.signals2 ===" << std::endl;

	WeatherData sensor;

	Display display1(std::cout);
	Display display2(std::cout);
	StatsDisplay statsDisplay(std::cout);

	// Display1 подписывается на температуру и давление с приоритетом 1
	auto conn1 = sensor.DoOnTemperatureChanged(
		[&display1](const WeatherInfo& data) { display1.OnTemperatureChanged(data); }, 1);
	auto conn2 = sensor.DoOnPressureChanged(
		[&display1](const WeatherInfo& data) { display1.OnPressureChanged(data); }, 1);

	// Display2 подписывается на температуру, влажность, скорость и направление ветра с приоритетом 2
	auto conn3 = sensor.DoOnTemperatureChanged(
		[&display2](const WeatherInfo& data) { display2.OnTemperatureChanged(data); }, 2);
	auto conn4 = sensor.DoOnHumidityChanged(
		[&display2](const WeatherInfo& data) { display2.OnHumidityChanged(data); }, 2);
	auto conn5 = sensor.DoOnWindSpeedChanged(
		[&display2](const WeatherInfo& data) { display2.OnWindSpeedChanged(data); }, 2);
	auto conn6 = sensor.DoOnWindDirectionChanged(
		[&display2](const WeatherInfo& data) { display2.OnWindDirectionChanged(data); }, 2);

	// StatsDisplay подписывается на все события с приоритетом 0
	auto conn7 = sensor.DoOnTemperatureChanged(
		[&statsDisplay](const WeatherInfo& data) { statsDisplay.OnTemperatureChanged(data); }, 0);
	auto conn8 = sensor.DoOnHumidityChanged(
		[&statsDisplay](const WeatherInfo& data) { statsDisplay.OnHumidityChanged(data); }, 0);
	auto conn9 = sensor.DoOnPressureChanged(
		[&statsDisplay](const WeatherInfo& data) { statsDisplay.OnPressureChanged(data); }, 0);
	auto conn10 = sensor.DoOnWindSpeedChanged(
		[&statsDisplay](const WeatherInfo& data) { statsDisplay.OnWindSpeedChanged(data); }, 0);
	auto conn11 = sensor.DoOnWindDirectionChanged(
		[&statsDisplay](const WeatherInfo& data) { statsDisplay.OnWindDirectionChanged(data); }, 0);

	std::cout << "\n--- Update temperature to -20 (display2, display1, statsDisplay should react) ---" << std::endl;
	sensor.SetTemperature(-20);

	std::cout << "\n--- Update humidity to 0.7 (display2, statsDisplay should react) ---" << std::endl;
	sensor.SetHumidity(0.7);

	std::cout << "\n--- Update pressure to 766 (display1, statsDisplay should react) ---" << std::endl;
	sensor.SetPressure(766);

	std::cout << "\n--- Update wind speed to 15 (display2, statsDisplay should react) ---" << std::endl;
	sensor.SetWindSpeed(15);

	std::cout << "\n--- Update wind direction to 180 (display2, statsDisplay should react) ---" << std::endl;
	sensor.SetWindDirection(180);

	// Демонстрация отписки
	std::cout << "\n--- Unsubscribing display2 from WindSpeed events ---" << std::endl;
	conn5.disconnect();

	std::cout << "\n--- Update wind speed to 20 (only statsDisplay should react) ---" << std::endl;
	sensor.SetWindSpeed(20);

	std::cout << "\n--- Update wind direction to 90 (display2, statsDisplay should still react) ---" << std::endl;
	sensor.SetWindDirection(90);

	std::cout << "\n--- SetMeasurements test (all subscribers react to their events) ---" << std::endl;
	sensor.SetMeasurements(25, 0.65, 755, 10, 270);
}

int main()
{
	TestWeatherStation();
	return 0;
}