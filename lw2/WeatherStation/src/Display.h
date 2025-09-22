#pragma once
#include "Observer.h"
#include "WeatherData.h"

class Display : public IObserver<WeatherInfo>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу Observable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(WeatherInfo const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

struct StatsInfo
{
	explicit StatsInfo(const std::string& name)
		: name(name)
	{
	}

	void Update(double value)
	{
		if (m_minValue > value)
		{
			m_minValue = value;
		}
		if (m_maxValue < value)
		{
			m_maxValue = value;
		}
		m_accValue += value;
		++m_countAcc;
	}

	void Print(std::ostream& out) const
	{
		out << "Max " << name << " " << m_maxValue << std::endl;
		out << "Min " << name << " " << m_minValue << std::endl;
		out << "Average " << name << " " << (m_accValue / m_countAcc) << std::endl;
		out << "----------------\n";
	}

	std::string name;
	double m_minValue = std::numeric_limits<double>::infinity();
	double m_maxValue = -std::numeric_limits<double>::infinity();
	double m_accValue = 0;
	double m_countAcc = 0;
};

class StatsDisplay final : public IObserver<WeatherInfo>
{
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
		m_temperatureInfo.Print(std::cout);
		m_humidityInfo.Print(std::cout);
		m_pressureInfo.Print(std::cout);
	}

	StatsInfo m_temperatureInfo{ "Temperature" };
	StatsInfo m_humidityInfo{ "Humidity" };
	StatsInfo m_pressureInfo{ "Pressure" };
};