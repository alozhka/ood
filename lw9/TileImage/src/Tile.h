#pragma once
#include <array>
#include <cassert>

#include "Geom.h"

class Tile
{
public:
	// Размер тайла 8*8 пикселей.
	constexpr static int SIZE = 8;

	// Конструктор по умолчанию. Заполняет тайл указанным цветом.
	explicit Tile(char color = ' ') noexcept
	{
		for (size_t i = 0; i < SIZE; ++i)
		{
			for (size_t j = 0; j < SIZE; ++j)
			{
				m_pixels[i][j] = color;
			}
		}
		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	Tile(const Tile& other)
		: m_pixels(other.m_pixels)
	{
		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	~Tile()
	{
		// -------------- не удалять ------------
		--m_instanceCount; // Уменьшаем счётчик тайлов.
		assert(m_instanceCount >= 0);
		// -------------- не удалять ------------
	}

	/**
	 * Изменяет цвет пикселя тайла.
	 * Если координаты выходят за пределы тайла, метод ничего не делает.
	 */
	void SetPixel(Point p, char color) noexcept
	{
		if (IsInRange(p))
		{
			m_pixels[p.x][p.y] = color;
		}
	}

	/**
	 * Возвращает цвет пикселя. Если координаты выходят за пределы тайла, возвращается пробел.
	 */
	char GetPixel(Point p) const noexcept
	{
		if (!IsInRange(p))
		{
			return ' ';
		}
		return m_pixels[p.x][p.y];
	}

	// Возвращает количество экземпляра класса Tile в программе.
	static int GetInstanceCount() noexcept
	{
		// -------------- не удалять ------------
		return m_instanceCount;
		// -------------- не удалять ------------
	}

private:
	static bool IsInRange(Point p)
	{
		return p.x >= 0 && p.y >= 0 && p.x < SIZE && p.y < SIZE;
	}
	// -------------- не удалять ------------
	inline static int m_instanceCount = 0;
	// -------------- не удалять ------------

	/* Разместите здесь поля для хранения пикселей тайла. */
	std::array<std::array<char, SIZE>, SIZE> m_pixels{};
};
