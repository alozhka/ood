#pragma once
#include "../src/Observer.h"
#include "../src/WeatherData.h"
#include <vector>

// Отслеживает порядкок вызова наблюдателей
class OrderTestDisplay : public IObserver<WeatherInfo>
{
public:
	OrderTestDisplay(std::vector<int>& callOrder, int id)
		: m_callOrder(callOrder)
		, m_id(id)
	{
	}

	void Update(const WeatherInfo& data, IObservable<WeatherInfo>& source) override
	{
		m_callOrder.push_back(m_id);
	}

private:
	std::vector<int>& m_callOrder;
	int m_id;
};