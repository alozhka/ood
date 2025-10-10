#include "src/Display.h"
#include "src/WeatherData.h"

int main()
{
	WeatherData sensor;

	Display display1(std::cout);
	Display display2(std::cout);

	sensor.RegisterObserver(display1, 1, WeatherType::Temperature);
	sensor.RegisterObserver(display1, 1, WeatherType::Humidity);
	sensor.RegisterObserver(display2, 2, WeatherType::Temperature);
	sensor.RegisterObserver(display2, 2, WeatherType::Temperature);

	sensor.SetTemperature(-20);
	sensor.SetTemperature(0);
	sensor.SetHumidity(0.2);
	sensor.SetPressure(766);

	return 0;
}