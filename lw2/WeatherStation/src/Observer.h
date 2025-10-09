#pragma once

#include <map>

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObserver
{
public:
	virtual void Update(T const& data) = 0;
	virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<T>& observer, int priority) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(IObserver<T>& observer) = 0;
};

template <class T>
class Observable : public IObservable<T>
{
public:
	using ObserverType = IObserver<T>;

	void RegisterObserver(ObserverType& observer, int priority) override
	{
		auto it = m_observerPriorities.find(&observer);
		if (it != m_observerPriorities.end())
		{
			return;
		}

		// NOTE: std::multimap сортирует по возрастанию ключа, а нужно по убыванию приоритета
		m_observers.emplace(-priority, &observer);
		m_observerPriorities.emplace(&observer, priority);
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		// NOTE: копирование предотвращает неопределённое поведение при удалении наблюдателя самим собой
		std::multimap<int, ObserverType*> observersCopy(m_observers);

		for (auto& [priority, observer] : observersCopy)
		{
			observer->Update(data);
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		auto it = m_observerPriorities.find(&observer);
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
	virtual T GetChangedData() const = 0;

private:
	std::multimap<int, ObserverType*> m_observers;
	std::unordered_map<ObserverType*, int> m_observerPriorities;
};
