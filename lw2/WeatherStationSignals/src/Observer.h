#pragma once

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename TData, typename TEvent>
class IObserver
{
public:
	virtual void Update(const TData& data, const TEvent& eventType) = 0;
	virtual ~IObserver() = default;
};