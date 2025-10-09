#pragma once
#include "Observer.h"
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

struct StatsInfo
{
	explicit StatsInfo(const std::string& name)
		: m_name(name)
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
		out << "Max " << m_name << " " << m_maxValue << std::endl;
		out << "Min " << m_name << " " << m_minValue << std::endl;
		out << "Average " << m_name << " " << (m_accValue / m_countAcc) << std::endl;
		out << "---------------------\n";
	}

	std::string m_name;
	double m_minValue = std::numeric_limits<double>::infinity();
	double m_maxValue = -std::numeric_limits<double>::infinity();
	double m_accValue = 0;
	uint m_countAcc = 0;
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

		PrintSensorLocation(source);
		m_temperatureInfo.Print(std::cout);
		m_humidityInfo.Print(std::cout);
		m_pressureInfo.Print(std::cout);
	}

	StatsInfo m_temperatureInfo{ "Temperature" };
	StatsInfo m_humidityInfo{ "Humidity" };
	StatsInfo m_pressureInfo{ "Pressure" };

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
};