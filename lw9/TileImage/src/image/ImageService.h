#pragma once
#include "Image.h"
#include <cstdint>

class ImageService
{
public:
	/**
	 * Выводит в поток out изображение в виде символов (для совместимости).
	 * Для цветных изображений выводит самый старший байт как символ.
	 */
	static void Print(const Image& img, std::ostream& out)
	{
		const auto size = img.GetSize();
		for (int y = 0; y < size.height; ++y)
		{
			for (int x = 0; x < size.width; ++x)
			{
				out.put(static_cast<char>(img.GetPixel({ x, y })));
			}
			out.put('\n');
		}
	}

	/**
	 * Загружает изображение из pixels. Линии изображения разделяются символами \n.
	 * Размеры картинки определяются по количеству переводов строки и самой длинной линии.
	 */
	static Image LoadImage(const std::string& pixels)
	{
		std::istringstream s(pixels);
		Size size;
		std::string line;
		while (std::getline(s, line))
		{
			size.width = std::max(size.width, static_cast<unsigned int>(line.length()));
			++size.height;
		}

		Image img(size);

		s = std::istringstream(pixels);
		for (int y = 0; y < size.height; ++y)
		{
			if (!std::getline(s, line))
				break;

			int x = 0;
			for (char ch : line)
			{
				img.SetPixel({ x++, y }, ch);
			}
		}

		return img;
	}

	/**
	 * Сохраняет изображение в поток out в формате PPM (P3 - ASCII).
	 * Формат RGB со значениями от 0 до 255.
	 */
	static void SaveToPPM(const Image& img, std::ostream& out)
	{
		const auto size = img.GetSize();

		out << "P3\n"
			<< size.width << " " << size.height << "\n"
			<< "255\n";

		for (int y = 0; y < size.height; ++y)
		{
			for (int x = 0; x < size.width; ++x)
			{
				uint32_t pixel = img.GetPixel({ x, y });

				uint8_t r = pixel >> 16 & 0xFF;
				uint8_t g = pixel >> 8 & 0xFF;
				uint8_t b = pixel & 0xFF;

				out << static_cast<int>(r) << " "
					<< static_cast<int>(g) << " "
					<< static_cast<int>(b);

				if (x < size.width - 1)
				{
					out << " ";
				}
			}
			out << std::endl;
		}
	}
};