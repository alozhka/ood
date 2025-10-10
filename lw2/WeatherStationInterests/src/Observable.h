#pragma once
#include "Observer.h"

#include <map>

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename TData, typename TEvent>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<TData, TEvent>& observer, int priority, TEvent eventType) = 0;
	virtual void NotifyObservers(TEvent eventType) = 0;
	virtual void RemoveObserver(IObserver<TData, TEvent>& observer, TEvent eventType) = 0;
};

template <class TData, class TEvent>
class PriorityObservable : public IObservable<TData, TEvent>
{
public:
	using ObserverType = IObserver<TData, TEvent>;

	void RegisterObserver(ObserverType& observer, int priority, TEvent eventType) override
	{
		ObserverInfo info{ &observer, eventType };
		auto it = m_observerPriorities.find(info);
		if (it != m_observerPriorities.end())
		{
			return;
		}

		// NOTE: std::multimap сортирует по возрастанию ключа, а нужно по убыванию приоритета
		m_observers.emplace(-priority, &observer);
		m_observerPriorities.emplace(&observer, priority);
	}

	void NotifyObservers(TEvent eventType) override
	{
		TData data = GetChangedData();
		std::vector<ObserverType*> observersCopy = ListObserversByEvent(eventType);

		for (auto& observer : observersCopy)
		{
			observer->Update(data, eventType);
		}
	}

	void RemoveObserver(ObserverType& observer, TEvent eventType) override
	{
		ObserverInfo info{ observer, eventType };
		auto it = m_observerPriorities.find(&info);
		if (it == m_observerPriorities.end())
		{
			return;
		}

		int priority = it->second;
		m_observerPriorities.erase(it);

		auto range = m_observers.equal_range(-priority);
		for (auto iter = range.first; iter != range.second; ++iter)
		{
			if (iter->second == &observer)
			{
				m_observers.erase(iter);
				break;
			}
		}
	}

protected:
	// Классы-наследники должны перегрузить данный метод,
	// в котором возвращать информацию об изменениях в объекте
	virtual TData GetChangedData() const;

private:
	std::vector<ObserverType*> ListObserversByEvent(TEvent eventType) const
	{
		std::vector<ObserverType*> observers;

		for (auto& [priority, info] : m_observers)
		{
			if (info.eventType == eventType)
			{
				observers.push_back(info.observer);
			}
		}

		return observers;
	}

	struct ObserverInfo
	{
		ObserverType* observer;
		TEvent eventType;

		bool operator==(const ObserverInfo& other) const
		{
			return observer == other.observer && eventType == other.eventType;
		}
	};

	struct ObserverInfoHasher
	{
		size_t operator()(const ObserverInfo& info) const
		{
			return std::hash<ObserverType*>()(info.observer) * 41 + std::hash<TEvent>()(info.eventType);
		}
	};

	using ObserversMultimap = std::multimap<int, ObserverInfo>;

	ObserversMultimap m_observers;
	std::unordered_map<ObserverInfo, typename ObserversMultimap::iterator, ObserverInfoHasher> m_observerPriorities;
};