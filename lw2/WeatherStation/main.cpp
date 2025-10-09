#include "src/Display.h"
#include "src/WeatherData.h"

int main()
{
	WeatherData wd;

	Display display;
	wd.RegisterObserver(display, 5);

	StatsDisplay statsDisplay;
	wd.RegisterObserver(statsDisplay, 10);

	wd.SetMeasurements(3, 0.7, 760);
	std::cout << std::endl;

	wd.SetMeasurements(4, 0.8, 761);
	std::cout << std::endl;

	wd.RegisterObserver(display, 5);
	wd.RemoveObserver(statsDisplay);

	wd.SetMeasurements(10, 0.8, 761);
	std::cout << std::endl;
	wd.SetMeasurements(-10, 0.8, 761);
	return 0;
}