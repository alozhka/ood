#pragma once

#include "Dance/DanceBehaviorFunctions.h"
#include "Fly/FlyBehaviorFunctions.h"
#include "Quack/QuackBehaviorFunctions.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>

struct FlyBehavior;

class Duck
{
public:
	Duck(std::unique_ptr<FlyBehavior>&& flyBehavior,
		std::unique_ptr<QuackBehavior>&& quackBehavior,
		std::unique_ptr<DanceBehavior>&& danceBehavior)
		: m_quackBehavior(std::move(quackBehavior))
		, m_danceBehavior(std::move(danceBehavior))
	{
		assert(m_quackBehavior);
		assert(m_danceBehavior);
		SetFlyBehavior(std::move(flyBehavior));
	}

	void Quack() const
	{
		m_quackBehavior->Quack();
	}

	void Swim() const
	{
		std::cout << "I'm swimming" << std::endl;
	}

	void Fly() const
	{
		if (!m_flyBehavior->IsFlyable())
		{
			return;
		}

		m_flyBehavior->Fly();

		if (m_quackBehavior->IsQuackable() && m_flyBehavior->GetFlyCount() % 2 == 0)
		{
			Quack();
		}
	}

	void Dance() const
	{
		(*m_danceBehavior)();
	}

	void SetFlyBehavior(std::unique_ptr<FlyBehavior>&& flyBehavior)
	{
		assert(flyBehavior);
		m_flyBehavior = std::move(flyBehavior);
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	std::unique_ptr<FlyBehavior> m_flyBehavior;
	std::unique_ptr<QuackBehavior> m_quackBehavior;
	std::unique_ptr<DanceBehavior> m_danceBehavior;
};