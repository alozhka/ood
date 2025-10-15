#pragma once

#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class Beverage : public IBeverage
{
public:
	explicit Beverage(const std::string& description)
		: m_description(description)
	{
	}

	std::string GetDescription() const final
	{
		return m_description;
	}

private:
	std::string m_description;
};

// Кофе
class Coffee : public Beverage
{
public:
	Coffee(const std::string& description = "Coffee")
		: Beverage(description)
	{
	}

	double GetCost() const override
	{
		return 60;
	}
};

class Cappuccino : public Coffee
{
public:
	explicit Cappuccino(bool isDouble = false)
		: Coffee(isDouble ? "Dobule Cappuccino" : "Cappuccino")
		, m_isDouble(isDouble)
	{
	}

	double GetCost() const override
	{
		return m_isDouble ? 120 : 80;
	}

private:
	bool m_isDouble;
};

class Latte : public Coffee
{
public:
	explicit Latte(bool isDouble = false)
		: Coffee(isDouble ? "Double Latte" : "Latte")
		, m_isDouble(isDouble)
	{
	}

	double GetCost() const override
	{
		return m_isDouble ? 130 : 90;
	}

private:
	bool m_isDouble;
};

class Tea : public Beverage
{
public:
	Tea()
		: Beverage("Tea")
	{
	}

	double GetCost() const override
	{
		return 30;
	}
};

enum class MilkshakeSize
{
	Small,
	Medium,
	Large,
};

class Milkshake : public Beverage
{
public:
	explicit Milkshake(MilkshakeSize size)
		: Beverage(MilkshakeSizeToString(size) + " milkshake")
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return MilkshakeSizeToCost(m_size);
	}

private:
	static std::string MilkshakeSizeToString(MilkshakeSize size)
	{
		switch (size)
		{
		case MilkshakeSize::Small:
			return "Small";
		case MilkshakeSize::Medium:
			return "Medium";
		case MilkshakeSize::Large:
			return "Large";
		default:
			throw std::runtime_error("Unknown milkshake size");
		}
	}

	static double MilkshakeSizeToCost(MilkshakeSize size)
	{
		switch (size)
		{
		case MilkshakeSize::Small:
			return 50;
		case MilkshakeSize::Medium:
			return 60;
		case MilkshakeSize::Large:
			return 80;
		default:
			throw std::runtime_error("Unknown milkshake size");
		}
	}

	MilkshakeSize m_size;
};
