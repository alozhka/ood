#pragma once
#include "Observer.h"

#include <map>
#include <unordered_map>

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
	virtual void RemoveObserver(IObserver<TData, TEvent>& observer, TEvent eventType) = 0;
};

template <class TData, class TEvent>
class PriorityObservable : public IObservable<TData, TEvent>
{
public:
	using ObserverType = IObserver<TData, TEvent>;
	using ObserverEventKey = std::pair<ObserverType*, TEvent>;

	void RegisterObserver(ObserverType& observer, int priority, TEvent eventType) override
	{
		// Ключ для хранения подписки: комбинация наблюдателя и типа события
		ObserverEventKey key = std::make_pair(&observer, eventType);

		// Проверяем, не подписан ли уже наблюдатель на это событие
		auto it = m_observerPriorities.find(key);
		if (it != m_observerPriorities.end())
		{
			return;
		}

		// NOTE: std::multimap сортирует по возрастанию ключа, а нужно по убыванию приоритета
		m_observers[eventType].emplace(-priority, &observer);
		m_observerPriorities.emplace(key, priority);
	}

	void NotifyObservers(TEvent eventType)
	{
		TData data = GetChangedData();

		auto it = m_observers.find(eventType);
		if (it == m_observers.end())
		{
			return;
		}

		// NOTE: копирование предотвращает неопределённое поведение при удалении наблюдателя самим собой
		std::multimap<int, ObserverType*> observersCopy(it->second);

		for (auto& [priority, observer] : observersCopy)
		{
			observer->Update(data, eventType);
		}
	}

	void RemoveObserver(ObserverType& observer, TEvent eventType) override
	{
		ObserverEventKey key = std::make_pair(&observer, eventType);
		auto it = m_observerPriorities.find(key);
		if (it == m_observerPriorities.end())
		{
			return;
		}

		int priority = it->second;
		m_observerPriorities.erase(it);

		auto& eventObservers = m_observers[eventType];
		auto range = eventObservers.equal_range(-priority);
		for (auto iter = range.first; iter != range.second; ++iter)
		{
			if (iter->second == &observer)
			{
				eventObservers.erase(iter);
				break;
			}
		}

		// Если для данного типа события больше нет наблюдателей, удаляем сам ключ
		if (eventObservers.empty())
		{
			m_observers.erase(eventType);
		}
	}

protected:
	// Классы-наследники должны перегрузить данный метод,
	// в котором возвращать информацию об изменениях в объекте
	virtual TData GetChangedData() const = 0;

private:
	// Хранилище наблюдателей: тип события -> (приоритет -> наблюдатель)
	std::map<TEvent, std::multimap<int, ObserverType*>> m_observers;

	// Хранилище приоритетов: (наблюдатель, тип события) -> приоритет
	std::map<ObserverEventKey, int> m_observerPriorities;
};
