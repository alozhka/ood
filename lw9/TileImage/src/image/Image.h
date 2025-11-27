#pragma once
#include "../CoW.h"
#include "../Geom.h"
#include "../Tile.h"

#include <sstream>
#include <string>

class Image
{
public:
	/**
	 * Конструирует изображение заданного размера. Если размеры не являются положительными,
	 * выбрасывает исключение std::out_of_range.
	 */
	explicit Image(Size size, char color = ' ')
		: m_size(size)
		, m_color(color)
		, m_tiles(size.width, std::vector(size.height, CoW(Tile(color))))
	{
	}

	// Возвращает размер изображения в пикселях.
	Size GetSize() const noexcept
	{
		return m_size;
	}

	/**
	 * Возвращает «цвет» пикселя в указанных координатах. Если координаты выходят за пределы
	 * изображения, возвращает «пробел».
	 */
	char GetPixel(Point p) const noexcept
	{
		if (!p.IsInSize(m_size))
		{
			return ' ';
		}

		int tileX = p.x / Tile::SIZE;
		int tileY = p.y / Tile::SIZE;
		int pixelX = p.x % Tile::SIZE;
		int pixelY = p.y % Tile::SIZE;

		return m_tiles[tileX][tileY]->GetPixel({ pixelX, pixelY });
	}

	/**
	 * Задаёт «цвет» пикселя в указанных координатах. Если координаты выходят за пределы изображения
	 * действие игнорируется.
	 */
	void SetPixel(Point p, char color)
	{
		if (!p.IsInSize(m_size))
		{
			return;
		}

		int tileX = p.x / Tile::SIZE;
		int tileY = p.y / Tile::SIZE;
		int pixelX = p.x % Tile::SIZE;
		int pixelY = p.y % Tile::SIZE;

		m_tiles[tileX][tileY].Write([&](Tile& tile) {
			tile.SetPixel({ pixelX, pixelY }, color);
		});
	}

private:
	Size m_size;
	char m_color;
	std::vector<std::vector<CoW<Tile>>> m_tiles;
};
