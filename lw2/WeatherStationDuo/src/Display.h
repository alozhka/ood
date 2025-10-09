#pragma once
#include "Observer.h"
#include "StatsInfo.h"
#include "WeatherData.h"

class Display final : public IObserver<WeatherInfo>
	, public IObserver<OutWeatherInfo>
{
public:
	explicit Display(
		std::ostream& output,
		int priority,
		IObservable<WeatherInfo>& indoorObserver,
		IObservable<OutWeatherInfo>& outdoorObserver)
		: m_output(output)
		, m_indoorObserver(indoorObserver)
		, m_outdoorObserver(outdoorObserver)
	{
		m_indoorObserver.RegisterObserver(*this, priority);
		m_outdoorObserver.RegisterObserver(*this, priority);
	}

	~Display() override
	{
		m_indoorObserver.RemoveObserver(*this);
		m_outdoorObserver.RemoveObserver(*this);
	}

private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу Observable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(WeatherInfo const& data) override
	{
		m_output << "-------Indoor--------\n";
		PrintInfo(data);
	}

	void Update(const OutWeatherInfo& data) override
	{
		m_output << "-------Outdoor-------\n";
		PrintInfo(data);
	}

	void PrintInfo(const WeatherInfo& data) const
	{
		m_output << "Current Temp " << data.temperature << std::endl;
		m_output << "Current Hum " << data.humidity << std::endl;
		m_output << "Current Pressure " << data.pressure << std::endl;
		m_output << "---------------------\n";
	}

	std::ostream& m_output;
	IObservable<WeatherInfo>& m_indoorObserver;
	IObservable<OutWeatherInfo>& m_outdoorObserver;
};

class StatsDisplay final : public IObserver<WeatherInfo>
	, public IObserver<OutWeatherInfo>
{
public:
	explicit StatsDisplay(
		std::ostream& output,
		int priority,
		IObservable<WeatherInfo>& indoorObserver,
		IObservable<OutWeatherInfo>& outdoorObserver)
		: m_output(output)
		, m_indoorObserver(indoorObserver)
		, m_outdoorObserver(outdoorObserver)
	{
		m_indoorObserver.RegisterObserver(*this, priority);
		m_outdoorObserver.RegisterObserver(*this, priority);
	}

	~StatsDisplay() override
	{
		m_indoorObserver.RemoveObserver(*this);
		m_outdoorObserver.RemoveObserver(*this);
	}

private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу Observable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(WeatherInfo const& data) override
	{
		m_temperatureInfo.Update(data.temperature);
		m_humidityInfo.Update(data.humidity);
		m_pressureInfo.Update(data.pressure);

		m_output << "-------Indoor--------\n";
		m_temperatureInfo.Print(std::cout);
		m_humidityInfo.Print(std::cout);
		m_pressureInfo.Print(std::cout);
		m_windSpeedInfo.Print(std::cout);
		m_windDirectionInfo.Print(std::cout);
	}

	void Update(const OutWeatherInfo& data) override
	{
		m_temperatureInfo.Update(data.temperature);
		m_humidityInfo.Update(data.humidity);
		m_pressureInfo.Update(data.pressure);
		m_windSpeedInfo.Update(data.windSpeed);
		m_windDirectionInfo.Update(data.windDirection);

		m_output << "-------Outdoor-------\n";
		m_temperatureInfo.Print(std::cout);
		m_humidityInfo.Print(std::cout);
		m_pressureInfo.Print(std::cout);
		m_windSpeedInfo.Print(std::cout);
		m_windDirectionInfo.Print(std::cout);
	}

	std::ostream& m_output;
	IObservable<WeatherInfo>& m_indoorObserver;
	IObservable<OutWeatherInfo>& m_outdoorObserver;

	StatsInfo m_temperatureInfo{ "Temperature" };
	StatsInfo m_humidityInfo{ "Humidity" };
	StatsInfo m_pressureInfo{ "Pressure" };
	StatsInfo m_windSpeedInfo{ "Wind speed" };
	WindDirectionInfo m_windDirectionInfo;
};