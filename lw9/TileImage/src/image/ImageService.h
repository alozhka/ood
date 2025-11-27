#pragma once
#include "Image.h"

class ImageService
{
public:
	/**
	 * Выводит в поток out изображение в виде символов.
	 */
	static void Print(const Image& img, std::ostream& out)
	{
		const auto size = img.GetSize();
		for (int y = 0; y < size.height; ++y)
		{
			for (int x = 0; x < size.width; ++x)
			{
				out.put(img.GetPixel({ x, y }));
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
			size.width = std::max(size.width, static_cast<int>(line.length()));
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
};