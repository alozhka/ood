#pragma once
#include "../src/Observer.h"
#include "../src/WeatherData.h"

class BrokenDisplay : public IObserver<WeatherInfo>
{
public:
	explicit BrokenDisplay(WeatherData& weatherData, int priority = 0)
		: m_data(weatherData)
	{
		m_data.RegisterObserver(*this, priority);
	}

	int GetCallCount() const
	{
		return m_count;
	}

private:
	void Update(const WeatherInfo& data) override
	{
		++m_count;
		m_data.RemoveObserver(*this);
	}

	WeatherData& m_data;
	int m_count = 0;
};
