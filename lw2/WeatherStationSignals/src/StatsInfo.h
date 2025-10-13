#pragma once
#include <cmath>
#include <ostream>
#include <cstdint>

struct StatsInfo
{
	explicit StatsInfo(const std::string& name)
		: m_name(name)
	{
	}

	void Update(double value)
	{
		if (m_minValue > value)
		{
			m_minValue = value;
		}
		if (m_maxValue < value)
		{
			m_maxValue = value;
		}
		m_accValue += value;
		++m_countAcc;
	}

	void Print(std::ostream& out) const
	{
		out << "Max " << m_name << " " << m_maxValue << std::endl;
		out << "Min " << m_name << " " << m_minValue << std::endl;
		out << "Average " << m_name << " " << (m_accValue / m_countAcc) << std::endl;
		out << "---------------------\n";
	}

	std::string m_name;
	double m_minValue = std::numeric_limits<double>::infinity();
	double m_maxValue = -std::numeric_limits<double>::infinity();
	double m_accValue = 0;
	unsigned int m_countAcc = 0;
};

// Класс для вычисления статистики направления ветра
// Использует векторное усреднение для корректного расчета среднего направления
class WindDirectionInfo
{
public:
	void Update(double direction)
	{
		double radians = DegreesToRadians(direction);

		double x = std::cos(radians);
		double y = std::sin(radians);

		m_sumX += x;
		m_sumY += y;
		++m_count;
	}

	void Print(std::ostream& out) const
	{
		double radians = std::atan2(m_sumY, m_sumX);
		double degrees = RadiansToDegrees(radians);
		degrees = NormalizeAngle(degrees);

		out << "Average wind direction " << degrees << std::endl;
	}

private:
	static double DegreesToRadians(double degrees)
	{
		return degrees * M_PI / 180.0;
	}

	static double RadiansToDegrees(double radians)
	{
		return radians * 180.0 / M_PI;
	}

	static double NormalizeAngle(double angle)
	{
		while (angle > 360.0)
		{
			angle -= 360.0;
		}
		while (angle < 0.0)
		{
			angle += 360.0;
		}

		return angle;
	}

	double m_sumX = 0.0; // Сумма x-компонент векторов
	double m_sumY = 0.0; // Сумма y-компонент векторов
	unsigned int m_count = 0; // Количество измерений
};