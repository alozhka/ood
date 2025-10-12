#pragma once

#include "Observable.h"

struct WeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
	double windSpeed = 0;
	double windDirection = 0;
};

enum class WeatherType
{
	Temperature,
	Humidity,
	Pressure,
	WindSpeed,
	WindDirection
};

class WeatherData : public PriorityObservable<WeatherInfo, WeatherType>
{
public:
	double GetTemperature() const
	{
		return m_temperature;
	}

	double GetHumidity() const
	{
		return m_humidity;
	}

	double GetPressure() const
	{
		return m_pressure;
	}

	double GetWindSpeed() const
	{
		return m_windSpeed;
	}

	double GetWindDirection() const
	{
		return m_windDirection;
	}

	void SetTemperature(double temperature)
	{
		m_temperature = temperature;
		NotifyObservers(WeatherType::Temperature);
	}

	void SetHumidity(double humidity)
	{
		m_humidity = humidity;
		NotifyObservers(WeatherType::Humidity);
	}

	void SetPressure(double pressure)
	{
		m_pressure = pressure;
		NotifyObservers(WeatherType::Pressure);
	}

	void SetWindSpeed(double windSpeed)
	{
		m_windSpeed = windSpeed;
		NotifyObservers(WeatherType::WindSpeed);
	}

	void SetWindDirection(double windDirection)
	{
		m_windDirection = windDirection;
		NotifyObservers(WeatherType::WindDirection);
	}

	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		SetTemperature(temp);
		SetHumidity(humidity);
		SetPressure(pressure);
		SetWindSpeed(windSpeed);
		SetWindDirection(windDirection);
	}

protected:
	WeatherInfo GetChangedData() const override
	{
		WeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		info.windSpeed = GetWindSpeed();
		info.windDirection = GetWindDirection();
		return info;
	}

private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
	double m_windSpeed = 0;
	double m_windDirection = 0;
};
