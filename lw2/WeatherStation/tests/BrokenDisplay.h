#pragma once
#include "../src/Observer.h"
#include "../src/WeatherData.h"

class BrokenDisplay : public IObserver<WeatherInfo>
{
public:
	explicit BrokenDisplay(std::ostream& output, WeatherData& weatherData, int priority = 0)
		: m_data(weatherData)
		, m_output(output)
	{
		m_data.RegisterObserver(*this, priority);
	}

	void Update(const WeatherInfo& data, IObservable<WeatherInfo>& source) override
	{
		m_output << "temp: " << data.temperature
				 << ", humidity: " << data.humidity
				 << ", pressure: " << data.pressure << std::endl;
		m_data.RemoveObserver(*this);
	}

private:
	WeatherData& m_data;
	std::ostream& m_output;
};
