#include "../src/WeatherData.h"
#include "../src/AnimalWorld.h"

#include "gtest/gtest.h"
#include <vector>

class CountingObserver : public IObserver<WeatherInfo, WeatherType>
{
public:
	int GetCallCount() const { return m_count; }
	WeatherType GetLastEvent() const { return m_lastEvent; }

private:
	void Update(const WeatherInfo& data, const WeatherType& eventType) override
	{
		++m_count;
		m_lastEvent = eventType;
	}

	int m_count = 0;
	WeatherType m_lastEvent{};
};

class OrderTrackingObserver : public IObserver<WeatherInfo, WeatherType>
{
public:
	OrderTrackingObserver(std::vector<int>& order, int id)
		: m_order(order), m_id(id) {}

private:
	void Update(const WeatherInfo& data, const WeatherType& eventType) override
	{
		m_order.push_back(m_id);
	}

	std::vector<int>& m_order;
	int m_id;
};

class WeatherStationTests : public testing::Test
{
protected:
	WeatherData weatherData{};
};

TEST_F(WeatherStationTests, ObserverReceivesOnlySubscribedEvent)
{
	CountingObserver observer;
	weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, observer.GetCallCount());
	EXPECT_EQ(WeatherType::Temperature, observer.GetLastEvent());

	weatherData.SetHumidity(60.0);
	EXPECT_EQ(1, observer.GetCallCount());
}

TEST_F(WeatherStationTests, ObserverCanSubscribeToMultipleEvents)
{
	CountingObserver observer;
	weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);
	weatherData.RegisterObserver(observer, 1, WeatherType::Pressure);

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, observer.GetCallCount());

	weatherData.SetPressure(760.0);
	EXPECT_EQ(2, observer.GetCallCount());

	weatherData.SetHumidity(60.0);
	EXPECT_EQ(2, observer.GetCallCount());
}

TEST_F(WeatherStationTests, ObserversNotifiedByPriority)
{
	std::vector<int> callOrder;
	OrderTrackingObserver obs1(callOrder, 1);
	OrderTrackingObserver obs2(callOrder, 2);
	OrderTrackingObserver obs3(callOrder, 3);

	weatherData.RegisterObserver(obs1, 5, WeatherType::Temperature);
	weatherData.RegisterObserver(obs2, 10, WeatherType::Temperature);
	weatherData.RegisterObserver(obs3, 3, WeatherType::Temperature);

	weatherData.SetTemperature(25.0);

	ASSERT_EQ(3u, callOrder.size());
	EXPECT_EQ(2, callOrder[0]); // Приоритет 10
	EXPECT_EQ(1, callOrder[1]); // Приоритет 5
	EXPECT_EQ(3, callOrder[2]); // Приоритет 3
}

TEST_F(WeatherStationTests, UnsubscribeWorks)
{
	CountingObserver observer;
	weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, observer.GetCallCount());

	weatherData.RemoveObserver(observer, WeatherType::Temperature);

	weatherData.SetTemperature(30.0);
	EXPECT_EQ(1, observer.GetCallCount());
}

TEST_F(WeatherStationTests, DuplicateRegistrationIgnored)
{
	CountingObserver observer;
	weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);
	weatherData.RegisterObserver(observer, 5, WeatherType::Temperature);

	weatherData.SetTemperature(25.0);
	EXPECT_EQ(1, observer.GetCallCount());
}

TEST_F(WeatherStationTests, RemoveNonExistentObserverNoThrow)
{
	CountingObserver observer;
	EXPECT_NO_THROW(weatherData.RemoveObserver(observer, WeatherType::Temperature));
}