#pragma once

#include <boost/signals2.hpp>

using ScopedConnection = boost::signals2::connection;
template <typename Signature>
using Signal = boost::signals2::signal<Signature>;

struct WeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
	double windSpeed = 0;
	double windDirection = 0;
};

class WeatherData
{
public:
	using TemperatureSignal = Signal<void(const WeatherInfo&)>;
	using HumiditySignal = Signal<void(const WeatherInfo&)>;
	using PressureSignal = Signal<void(const WeatherInfo&)>;
	using WindSpeedSignal = Signal<void(const WeatherInfo&)>;
	using WindDirectionSignal = Signal<void(const WeatherInfo&)>;

	// Методы для подписки на конкретные события
	[[nodiscard]] ScopedConnection DoOnTemperatureChanged(const TemperatureSignal::slot_type& subscriber, int priority = 0)
	{
		return m_temperatureSignal.connect(-priority, subscriber);
	}

	[[nodiscard]] ScopedConnection DoOnHumidityChanged(const HumiditySignal::slot_type& subscriber, int priority = 0)
	{
		return m_humiditySignal.connect(-priority, subscriber);
	}

	[[nodiscard]] ScopedConnection DoOnPressureChanged(const PressureSignal::slot_type& subscriber, int priority = 0)
	{
		return m_pressureSignal.connect(-priority, subscriber);
	}

	[[nodiscard]] ScopedConnection DoOnWindSpeedChanged(const WindSpeedSignal::slot_type& subscriber, int priority = 0)
	{
		return m_windSpeedSignal.connect(-priority, subscriber);
	}

	[[nodiscard]] ScopedConnection DoOnWindDirectionChanged(const WindDirectionSignal::slot_type& subscriber, int priority = 0)
	{
		return m_windDirectionSignal.connect(-priority, subscriber);
	}

	// Геттеры
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

	// Сеттеры, которые вызывают соответствующие сигналы
	void SetTemperature(double temperature)
	{
		m_temperature = temperature;
		m_temperatureSignal(GetWeatherInfo());
	}

	void SetHumidity(double humidity)
	{
		m_humidity = humidity;
		m_humiditySignal(GetWeatherInfo());
	}

	void SetPressure(double pressure)
	{
		m_pressure = pressure;
		m_pressureSignal(GetWeatherInfo());
	}

	void SetWindSpeed(double windSpeed)
	{
		m_windSpeed = windSpeed;
		m_windSpeedSignal(GetWeatherInfo());
	}

	void SetWindDirection(double windDirection)
	{
		m_windDirection = windDirection;
		m_windDirectionSignal(GetWeatherInfo());
	}

	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		SetTemperature(temp);
		SetHumidity(humidity);
		SetPressure(pressure);
		SetWindSpeed(windSpeed);
		SetWindDirection(windDirection);
	}

private:
	WeatherInfo GetWeatherInfo() const
	{
		WeatherInfo info;
		info.temperature = m_temperature;
		info.humidity = m_humidity;
		info.pressure = m_pressure;
		info.windSpeed = m_windSpeed;
		info.windDirection = m_windDirection;
		return info;
	}

	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
	double m_windSpeed = 0.0;
	double m_windDirection = 0.0;

	TemperatureSignal m_temperatureSignal;
	HumiditySignal m_humiditySignal;
	PressureSignal m_pressureSignal;
	WindSpeedSignal m_windSpeedSignal;
	WindDirectionSignal m_windDirectionSignal;
};