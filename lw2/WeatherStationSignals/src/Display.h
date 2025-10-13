#pragma once
#include "WeatherData.h"
#include "StatsInfo.h"
#include <ostream>
#include <boost/signals2.hpp>

class Display
{
public:
	explicit Display(std::ostream& output)
		: m_output(output)
	{
	}

	void OnTemperatureChanged(const WeatherInfo& data)
	{
		m_output << "Current Temp: " << data.temperature << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnPressureChanged(const WeatherInfo& data)
	{
		m_output << "Current Pressure: " << data.pressure << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnHumidityChanged(const WeatherInfo& data)
	{
		m_output << "Current Hum: " << data.humidity << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnWindSpeedChanged(const WeatherInfo& data)
	{
		m_output << "Current Wind Speed: " << data.windSpeed << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnWindDirectionChanged(const WeatherInfo& data)
	{
		m_output << "Current Wind Direction: " << data.windDirection << std::endl;
		m_output << "----------------" << std::endl;
	}

private:
	std::ostream& m_output;
};

class StatsDisplay
{
public:
	explicit StatsDisplay(std::ostream& output)
		: m_output(output)
	{
	}

	void OnTemperatureChanged(const WeatherInfo& data)
	{
		m_temperatureInfo.Update(data.temperature);
		m_output << "=== Temperature Stats ===" << std::endl;
		m_temperatureInfo.Print(m_output);
	}

	void OnHumidityChanged(const WeatherInfo& data)
	{
		m_humidityInfo.Update(data.humidity);
		m_output << "=== Humidity Stats ===" << std::endl;
		m_humidityInfo.Print(m_output);
	}

	void OnPressureChanged(const WeatherInfo& data)
	{
		m_pressureInfo.Update(data.pressure);
		m_output << "=== Pressure Stats ===" << std::endl;
		m_pressureInfo.Print(m_output);
	}

	void OnWindSpeedChanged(const WeatherInfo& data)
	{
		m_windSpeedInfo.Update(data.windSpeed);
		m_output << "=== Wind Speed Stats ===" << std::endl;
		m_windSpeedInfo.Print(m_output);
	}

	void OnWindDirectionChanged(const WeatherInfo& data)
	{
		m_windDirectionInfo.Update(data.windDirection);
		m_output << "=== Wind Direction Stats ===" << std::endl;
		m_windDirectionInfo.Print(m_output);
	}

private:
	std::ostream& m_output;
	StatsInfo m_temperatureInfo{ "Temperature" };
	StatsInfo m_humidityInfo{ "Humidity" };
	StatsInfo m_pressureInfo{ "Pressure" };
	StatsInfo m_windSpeedInfo{ "Wind speed" };
	WindDirectionInfo m_windDirectionInfo;
};