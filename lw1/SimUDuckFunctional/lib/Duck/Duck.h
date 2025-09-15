#pragma once

#include "Dance/DanceBehaviorFunctions.h"
#include "Fly/FlyBehaviorFunctions.h"
#include "Quack/QuackBehaviorFunctions.h"

#include <cassert>
#include <functional>
#include <iostream>

class Duck
{
public:
	Duck(FlyFunction flyBehavior,
		QuackFunction quackBehavior,
		DanceFunction danceBehavior)
		: m_flyBehavior(std::move(flyBehavior))
		, m_quackBehavior(std::move(quackBehavior))
		, m_danceBehavior(std::move(danceBehavior))
	{
		assert(m_flyBehavior);
		assert(m_quackBehavior);
		assert(m_danceBehavior);
	}

	void Quack() const
	{
		m_quackBehavior();
	}

	void Swim() const
	{
		std::cout << "I'm swimming" << std::endl;
	}

	void Fly() const
	{
		auto callback = [this]() -> void {
			m_quackBehavior();
		};
		m_flyBehavior(callback);
	}

	void Dance() const
	{
		m_danceBehavior();
	}

	void SetFlyBehavior(const FlyFunction& flyBehavior)
	{
		assert(flyBehavior);
		m_flyBehavior = flyBehavior;
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	FlyFunction m_flyBehavior;
	QuackFunction m_quackBehavior;
	DanceFunction m_danceBehavior;
};
