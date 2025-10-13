#pragma once

#include "Observable.h"
#include <iostream>
#include <string>

// Информация о состоянии кота
struct CatInfo
{
	std::string location;
	std::string mood;
	bool isSleeping = false;
};

// Типы событий кота
enum class CatEvent
{
	LocationChanged,
	MoodChanged,
	SleepStateChanged
};

// Кот как наблюдаемый объект
class Cat : public PriorityObservable<CatInfo, CatEvent>
{
public:
	void MoveTo(const std::string& location)
	{
		m_location = location;
		NotifyObservers(CatEvent::LocationChanged);
	}

	void SetMood(const std::string& mood)
	{
		m_mood = mood;
		NotifyObservers(CatEvent::MoodChanged);
	}

	void GoToSleep()
	{
		m_isSleeping = true;
		NotifyObservers(CatEvent::SleepStateChanged);
	}

	void WakeUp()
	{
		m_isSleeping = false;
		NotifyObservers(CatEvent::SleepStateChanged);
	}

protected:
	CatInfo GetChangedData() const override
	{
		CatInfo info;
		info.location = m_location;
		info.mood = m_mood;
		info.isSleeping = m_isSleeping;
		return info;
	}

private:
	std::string m_location = "unknown";
	std::string m_mood = "neutral";
	bool m_isSleeping = false;
};

// Мышь - наблюдатель за котом
class Mouse : public IObserver<CatInfo, CatEvent>
{
public:
	explicit Mouse(const std::string& name, std::ostream& output)
		: m_name(name)
		, m_output(output)
	{
	}

private:
	void Update(const CatInfo& data, const CatEvent& eventType) override
	{
		switch (eventType)
		{
		case CatEvent::LocationChanged:
			m_output << "[Mouse " << m_name << "] Cat moved to " << data.location << "! Running away!" << std::endl;
			break;
		case CatEvent::SleepStateChanged:
			if (data.isSleeping)
			{
				m_output << "[Mouse " << m_name << "] Cat is sleeping. Time to look for food!" << std::endl;
			}
			else
			{
				m_output << "[Mouse " << m_name << "] Cat woke up! Hiding!" << std::endl;
			}
			break;
		case CatEvent::MoodChanged:
			// Мышь не интересуется настроением кота
			break;
		}
	}

	std::string m_name;
	std::ostream& m_output;
};

// Собака - наблюдатель за котом
class Dog : public IObserver<CatInfo, CatEvent>
{
public:
	explicit Dog(const std::string& name, std::ostream& output)
		: m_name(name)
		, m_output(output)
	{
	}

private:
	void Update(const CatInfo& data, const CatEvent& eventType) override
	{
		switch (eventType)
		{
		case CatEvent::LocationChanged:
			m_output << "[Dog " << m_name << "] Cat is at " << data.location << "! Let's play!" << std::endl;
			break;
		case CatEvent::MoodChanged:
			m_output << "[Dog " << m_name << "] Cat's mood: " << data.mood << std::endl;
			break;
		case CatEvent::SleepStateChanged:
			// Собаку не интересует, спит кот или нет
			break;
		}
	}

	std::string m_name;
	std::ostream& m_output;
};