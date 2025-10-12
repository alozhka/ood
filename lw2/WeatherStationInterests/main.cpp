#include "src/Display.h"
#include "src/WeatherData.h"
#include "src/AnimalWorld.h"

void TestWeatherStation()
{
	std::cout << "=== Weather Station Test ===" << std::endl;

	WeatherData sensor;

	Display display1(std::cout);
	Display display2(std::cout);

	sensor.RegisterObserver(display1, 1, WeatherType::Temperature);
	sensor.RegisterObserver(display1, 1, WeatherType::Pressure);

	sensor.RegisterObserver(display2, 2, WeatherType::Temperature);
	sensor.RegisterObserver(display2, 2, WeatherType::Humidity);
	sensor.RegisterObserver(display2, 2, WeatherType::WindSpeed);
	sensor.RegisterObserver(display2, 2, WeatherType::WindDirection);

	std::cout << "\nUpdate temperature to -20:" << std::endl;
	sensor.SetTemperature(-20);

	std::cout << "\nUpdate humidity to 0.7:" << std::endl;
	sensor.SetHumidity(0.7);

	std::cout << "\nUpdate pressure to 766:" << std::endl;
	sensor.SetPressure(766);

	std::cout << "\nUpdate wind speed to 15:" << std::endl;
	sensor.SetWindSpeed(15);

	std::cout << "\nUpdate wind direction to 180:" << std::endl;
	sensor.SetWindDirection(180);

	// Отписываемся от события ветра
	std::cout << "\nUnsubscribing display2 from WindSpeed events..." << std::endl;
	sensor.RemoveObserver(display2, WeatherType::WindSpeed);

	std::cout << "\nUpdate wind speed to 20 (display2 should not receive this):" << std::endl;
	sensor.SetWindSpeed(20);

	std::cout << "\nUpdate wind direction to 90 (display2 should still receive this):" << std::endl;
	sensor.SetWindDirection(90);
}

void TestAnimalWorld()
{
	std::cout << "\n\n=== Animal World Test ===" << std::endl;

	Cat cat;
	Mouse mouse1("Jerry", std::cout);
	Mouse mouse2("Stuart", std::cout);
	Dog dog("Spike", std::cout);

	// Мыши интересуются местоположением кота и его сном
	cat.RegisterObserver(mouse1, 1, CatEvent::LocationChanged);
	cat.RegisterObserver(mouse1, 1, CatEvent::SleepStateChanged);

	cat.RegisterObserver(mouse2, 2, CatEvent::LocationChanged);
	cat.RegisterObserver(mouse2, 2, CatEvent::SleepStateChanged);

	// Собака интересуется местоположением и настроением
	cat.RegisterObserver(dog, 3, CatEvent::LocationChanged);
	cat.RegisterObserver(dog, 3, CatEvent::MoodChanged);

	std::cout << "\nCat moves to kitchen:" << std::endl;
	cat.MoveTo("kitchen");

	std::cout << "\nCat goes to sleep:" << std::endl;
	cat.GoToSleep();

	std::cout << "\nCat's mood changes to angry:" << std::endl;
	cat.SetMood("angry");

	std::cout << "\nCat wakes up:" << std::endl;
	cat.WakeUp();

	// mouse1 больше не интересуется местоположением кота
	std::cout << "\nMouse Jerry unsubscribes from LocationChanged events..." << std::endl;
	cat.RemoveObserver(mouse1, CatEvent::LocationChanged);

	std::cout << "\nCat moves to bedroom (Jerry should not receive this):" << std::endl;
	cat.MoveTo("bedroom");

	std::cout << "\nCat goes to sleep (Jerry should still receive this):" << std::endl;
	cat.GoToSleep();
}

int main()
{
	TestWeatherStation();
	TestAnimalWorld();

	return 0;
}