#pragma once
#include "../src/Observer.h"
#include "../src/WeatherData.h"

class MockDisplay : public IObserver<WeatherInfo>
	, public IObserver<OutWeatherInfo>
{
public:
	explicit MockDisplay(WeatherData& weatherData, WeatherData& outWeatherData, int priority = 0)
		: m_data(weatherData)
		, m_outData(outWeatherData)
	{
		m_data.RegisterObserver(*this, priority);
		m_outData.RegisterObserver(*this, priority);
	}

	int GetCallCount() const
	{
		return m_count;
	}

private:
	void Update(const WeatherInfo& data) override
	{
		++m_count;
	}

	void Update(const OutWeatherInfo& data) override
	{
		++m_count;
	}

private:
	WeatherData& m_data;
	WeatherData& m_outData;
	int m_count = 0;
};
