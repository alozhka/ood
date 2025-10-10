#pragma once
#include "Observer.h"
#include "WeatherData.h"

class Display final : public IObserver<WeatherInfo, WeatherType>
{
public:
	explicit Display(std::ostream& output)
		: m_output(output)
	{
	}

private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу Observable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(const WeatherInfo& data, const WeatherType& eventType) override
	{
		switch (eventType)
		{
		case WeatherType::Temperature:
			PrintTemperature(data);
			break;
		case WeatherType::Humidity:
			PrintHumidity(data);
			break;
		case WeatherType::Pressure:
			PrintPressure(data);
			break;
		case WeatherType::WindSpeed:
			PrintWindSpeed(data);
			break;
		case WeatherType::WindDirection:
			PrintWindDirection(data);
			break;
		default:
			PrintInfo(data);
			break;
		}
		m_output << "----------------" << std::endl;
	}

	void PrintTemperature(WeatherInfo const& data) const
	{
		m_output << "Current Temp: " << data.temperature << std::endl;
	}

	void PrintPressure(WeatherInfo const& data) const
	{
		m_output << "Current Pressure: " << data.pressure << std::endl;
	}

	void PrintHumidity(WeatherInfo const& data) const
	{
		m_output << "Current Hum: " << data.humidity << std::endl;
	}

	void PrintWindSpeed(WeatherInfo const& data) const
	{
		m_output << "Current Wind Speed: " << data.windSpeed << std::endl;
	}

	void PrintWindDirection(WeatherInfo const& data) const
	{
		m_output << "Current Wind Direction " << data.windDirection << std::endl;
	}

	void PrintInfo(const WeatherInfo& data) const
	{
		PrintTemperature(data);
		PrintHumidity(data);
		PrintPressure(data);
		PrintWindSpeed(data);
		PrintWindDirection(data);
	}

	std::ostream& m_output;
};
