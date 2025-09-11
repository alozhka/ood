#pragma once

#include "../lib/Duck/Fly/IFlyBehavior.h"
#include <functional>

class MockFlyBehavior : public IFlyBehavior
{
public:
	MockFlyBehavior() 
		: m_flyCalled(false)
		, m_flyWithCallbackCalled(false)
		, m_callbackInvoked(false)
		, m_flightCount(0)
	{
	}

	void Fly() override
	{
		m_flyCalled = true;
		++m_flightCount;
	}

	void Fly(const std::function<void()>& onSpecialFlight) override
	{
		m_flyWithCallbackCalled = true;
		++m_flightCount;
		
		// Имитируем поведение: вызываем коллбек после каждого второго полёта
		if (m_flightCount % 2 == 0 && onSpecialFlight)
		{
			onSpecialFlight();
			m_callbackInvoked = true;
		}
	}

	bool WasFlyCalled() const { return m_flyCalled; }
	bool WasFlyWithCallbackCalled() const { return m_flyWithCallbackCalled; }
	bool WasCallbackInvoked() const { return m_callbackInvoked; }
	int GetFlightCount() const { return m_flightCount; }

	void Reset()
	{
		m_flyCalled = false;
		m_flyWithCallbackCalled = false;
		m_callbackInvoked = false;
		m_flightCount = 0;
	}

private:
	bool m_flyCalled;
	bool m_flyWithCallbackCalled;
	bool m_callbackInvoked;
	int m_flightCount;
};