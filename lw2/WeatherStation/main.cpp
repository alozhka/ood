#include "src/Display.h"
#include "src/WeatherData.h"

int main()
{
	WeatherData indoorSensor;
	WeatherData outdoorSensor;

	Display display(std::cout, indoorSensor, outdoorSensor);
	indoorSensor.RegisterObserver(display, 5);
	outdoorSensor.RegisterObserver(display, 5);

	StatsDisplay statsDisplay(std::cout, indoorSensor, outdoorSensor);
	indoorSensor.RegisterObserver(statsDisplay, 10);
	outdoorSensor.RegisterObserver(statsDisplay, 10);

	indoorSensor.SetMeasurements(3, 0.7, 760, 3, 30);
	std::cout << std::endl;

	outdoorSensor.SetMeasurements(4, 0.8, 761, 15, 45);
	std::cout << std::endl;

	indoorSensor.RegisterObserver(display, 5);
	indoorSensor.RemoveObserver(statsDisplay);

	indoorSensor.SetMeasurements(10, 0.8, 761, 10, 45);
	std::cout << std::endl;
	outdoorSensor.SetMeasurements(-10, 0.8, 761, 15, 45);
	return 0;
}