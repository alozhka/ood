#pragma once
#include "Observer.h"
#include "StatsInfo.h"
#include "WeatherData.h"

class Display : public IObserver<WeatherInfo>
{
public:
	explicit Display(
		std::ostream& output,
		IObservable<WeatherInfo>& indoorObserver,
		IObservable<WeatherInfo>& outdoorObserver)
		: m_output(output)
		, m_indoorObserver(indoorObserver)
		, m_outdoorObserver(outdoorObserver)
	{
	}

private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу Observable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(WeatherInfo const& data, IObservable<WeatherInfo>& source) override
	{
		PrintSensorLocation(source);
		m_output << "Current Temp " << data.temperature << std::endl;
		m_output << "Current Hum " << data.humidity << std::endl;
		m_output << "Current Pressure " << data.pressure << std::endl;
		m_output << "---------------------\n";
	}

	void PrintSensorLocation(IObservable<WeatherInfo>& observer) const
	{
		if (&observer == &m_indoorObserver)
		{
			m_output << "-------Indoor--------\n";
		}
		if (&observer == &m_outdoorObserver)
		{
			m_output << "-------Outdoor-------\n";
		}
	}

	std::ostream& m_output;
	IObservable<WeatherInfo>& m_indoorObserver;
	IObservable<WeatherInfo>& m_outdoorObserver;
};

class StatsDisplay final : public IObserver<WeatherInfo>
{
public:
	explicit StatsDisplay(
		std::ostream& output,
		IObservable<WeatherInfo>& indoorObserver,
		IObservable<WeatherInfo>& outdoorObserver)
		: m_output(output)
		, m_indoorObserver(indoorObserver)
		, m_outdoorObserver(outdoorObserver)
	{
	}

private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу Observable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(WeatherInfo const& data, IObservable<WeatherInfo>& source) override
	{
		m_temperatureInfo.Update(data.temperature);
		m_humidityInfo.Update(data.humidity);
		m_pressureInfo.Update(data.pressure);
		m_windSpeedInfo.Update(data.windSpeed);
		m_windDirectionInfo.Update(data.windDirection);

		PrintSensorLocation(source);
		m_temperatureInfo.Print(std::cout);
		m_humidityInfo.Print(std::cout);
		m_pressureInfo.Print(std::cout);
		m_windSpeedInfo.Print(std::cout);
		m_windDirectionInfo.Print(std::cout);
	}

	void PrintSensorLocation(IObservable<WeatherInfo>& observer) const
	{
		if (&observer == &m_indoorObserver)
		{
			std::cout << "-------Indoor--------\n";
		}
		if (&observer == &m_outdoorObserver)
		{
			std::cout << "-------Outdoor-------\n";
		}
	}

	std::ostream& m_output;
	IObservable<WeatherInfo>& m_indoorObserver;
	IObservable<WeatherInfo>& m_outdoorObserver;

	StatsInfo m_temperatureInfo{ "Temperature" };
	StatsInfo m_humidityInfo{ "Humidity" };
	StatsInfo m_pressureInfo{ "Pressure" };
	StatsInfo m_windSpeedInfo{ "Wind speed" };
	WindDirectionInfo m_windDirectionInfo;
};