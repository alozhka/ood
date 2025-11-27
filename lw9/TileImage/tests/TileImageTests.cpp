#include "../src/Geom.h"
#include "../src/Tile.h"
#include "../src/image/Image.h"
#include "../src/drawer/Drawer.h"
#include "../src/image/ImageService.h"
#include "gtest/gtest.h"
#include <sstream>

class ImageTests : public testing::Test
{
};

class TileTests : public testing::Test
{
};

TEST_F(TileTests, DefaultConstructorCreatesEmptyTile)
{
	ASSERT_EQ(Tile::GetInstanceCount(), 0);

	Tile tile;
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	for (int x = 0; x < Tile::SIZE; ++x)
	{
		for (int y = 0; y < Tile::SIZE; ++y)
		{
			EXPECT_EQ(tile.GetPixel({ x, y }), ' ');
		}
	}
}

TEST_F(TileTests, ConstructorWithColor)
{
	ASSERT_EQ(Tile::GetInstanceCount(), 0);

	Tile tile('#');
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	// Проверяем, что все пиксели заполнены указанным цветом
	for (int x = 0; x < Tile::SIZE; ++x)
	{
		for (int y = 0; y < Tile::SIZE; ++y)
		{
			EXPECT_EQ(tile.GetPixel({ x, y }), '#');
		}
	}
}

TEST_F(TileTests, CopyConstructor)
{
	ASSERT_EQ(Tile::GetInstanceCount(), 0);

	Tile original('@');
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	Tile copy(original);
	EXPECT_EQ(Tile::GetInstanceCount(), 2);

	// Проверяем, что копия содержит те же данные
	for (int x = 0; x < Tile::SIZE; ++x)
	{
		for (int y = 0; y < Tile::SIZE; ++y)
		{
			EXPECT_EQ(copy.GetPixel({ x, y }), '@');
		}
	}
}

TEST_F(TileTests, SetPixelInRange)
{
	Tile tile;

	// Устанавливаем пиксель в допустимых координатах
	tile.SetPixel({ 3, 4 }, '*');
	EXPECT_EQ(tile.GetPixel({ 3, 4 }), '*');

	// Устанавливаем несколько пикселей
	tile.SetPixel({ 0, 0 }, 'A');
	tile.SetPixel({ 7, 7 }, 'Z');

	EXPECT_EQ(tile.GetPixel({ 0, 0 }), 'A');
	EXPECT_EQ(tile.GetPixel({ 7, 7 }), 'Z');
	EXPECT_EQ(tile.GetPixel({ 3, 4 }), '*');
}

TEST_F(TileTests, SetPixelOutOfRange)
{
	Tile tile;

	// Пытаемся установить пиксель за пределами тайла
	tile.SetPixel({ -1, 0 }, 'X'); // Отрицательная координата
	tile.SetPixel({ 0, -1 }, 'X'); // Отрицательная координата
	tile.SetPixel({ 8, 0 }, 'X'); // Координата >= SIZE
	tile.SetPixel({ 0, 8 }, 'X'); // Координата >= SIZE

	// Все пиксели должны остаться пробелами
	for (int x = 0; x < Tile::SIZE; ++x)
	{
		for (int y = 0; y < Tile::SIZE; ++y)
		{
			EXPECT_EQ(tile.GetPixel({ x, y }), ' ');
		}
	}
}

TEST_F(TileTests, GetPixelOutOfRange)
{
	Tile tile('#');

	// Проверяем пиксели за пределами тайла
	EXPECT_EQ(tile.GetPixel({ -1, 0 }), ' ');
	EXPECT_EQ(tile.GetPixel({ 0, -1 }), ' ');
	EXPECT_EQ(tile.GetPixel({ 8, 0 }), ' ');
	EXPECT_EQ(tile.GetPixel({ 0, 8 }), ' ');
}

TEST_F(TileTests, DestructorUpdatesInstanceCount)
{
	ASSERT_EQ(Tile::GetInstanceCount(), 0);

	{
		Tile tile1;
		EXPECT_EQ(Tile::GetInstanceCount(), 1);

		{
			Tile tile2;
			EXPECT_EQ(Tile::GetInstanceCount(), 2);
		}

		EXPECT_EQ(Tile::GetInstanceCount(), 1);
	}

	EXPECT_EQ(Tile::GetInstanceCount(), 0);
}

TEST_F(ImageTests, ConstructorWithValidSize)
{
	Size size{ 10, 15 };
	Image image(size, '@');

	EXPECT_EQ(image.GetSize().width, 10);
	EXPECT_EQ(image.GetSize().height, 15);

	// Проверяем, что все пиксели имеют указанный цвет
	for (int x = 0; x < size.width; ++x)
	{
		for (int y = 0; y < size.height; ++y)
		{
			EXPECT_EQ(image.GetPixel({ x, y }), '@');
		}
	}
}

TEST_F(ImageTests, ConstructorWithDefaultColor)
{
	Size size{ 10, 10 };
	Image image(size);

	EXPECT_EQ(Tile::GetInstanceCount(), 1);
	// Проверяем, что все пиксели имеют цвет по умолчанию (пробел)
	for (int x = 0; x < size.width; ++x)
	{
		for (int y = 0; y < size.height; ++y)
		{
			EXPECT_EQ(image.GetPixel({ x, y }), ' ');
		}
	}
}

TEST_F(ImageTests, GetPixelOutOfRange)
{
	Size size{ 5, 5 };
	Image image(size, '#');

	// Проверяем пиксели за пределами изображения
	EXPECT_EQ(image.GetPixel({ -1, 0 }), ' ');
	EXPECT_EQ(image.GetPixel({ 0, -1 }), ' ');
	EXPECT_EQ(image.GetPixel({ 5, 0 }), ' ');
	EXPECT_EQ(image.GetPixel({ 0, 5 }), ' ');
	EXPECT_EQ(image.GetPixel({ 10, 10 }), ' ');
}

TEST_F(ImageTests, SetPixelInRange)
{
	Size size{ 16, 16 };
	Image image(size);

	// Устанавливаем пиксели в разных тайлах
	image.SetPixel({ 0, 0 }, 'A'); // Первый тайл
	image.SetPixel({ 8, 8 }, 'C'); // Второй тайл, левый верхний угол
	image.SetPixel({ 15, 15 }, 'D'); // Последний пиксель

	EXPECT_EQ(image.GetPixel({ 0, 0 }), 'A');
	EXPECT_EQ(image.GetPixel({ 8, 8 }), 'C');
	EXPECT_EQ(image.GetPixel({ 15, 15 }), 'D');
	EXPECT_EQ(image.GetPixel({ 1, 1 }), ' ');
}

TEST_F(ImageTests, SetPixelOutOfRange)
{
	Size size{ 5, 5 };
	Image image(size, '@');

	// Пытаемся установить пиксели за пределами изображения
	image.SetPixel({ -1, 0 }, 'X');
	image.SetPixel({ 0, -1 }, 'X');
	image.SetPixel({ 5, 0 }, 'X');
	image.SetPixel({ 0, 5 }, 'X');

	// Все пиксели должны остаться без изменений
	for (int x = 0; x < size.width; ++x)
	{
		for (int y = 0; y < size.height; ++y)
		{
			EXPECT_EQ(image.GetPixel({ x, y }), '@');
		}
	}
}

TEST_F(ImageTests, CopyOnWriteOptimization)
{
	ASSERT_EQ(Tile::GetInstanceCount(), 0);

	Size size{ 16, 16 };
	Image image1(size, '@');
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	Image image2 = image1;

	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	image2.SetPixel({ 0, 0 }, 'X');

	EXPECT_EQ(Tile::GetInstanceCount(), 2);

	// Первое изображение должно остаться неизменным
	EXPECT_EQ(image1.GetPixel({ 0, 0 }), '@');
	// Второе изображение должно содержать изменения
	EXPECT_EQ(image2.GetPixel({ 0, 0 }), 'X');
}

TEST_F(ImageTests, DrawCircleRadius4)
{
	// Создаем изображение 11x11 для окружности радиуса 4
	Size size{ 11, 11 };
	Image image(size);

	// Рисуем окружность радиуса 4 с центром в (5, 5)
	Drawer::DrawCircle(image, { 5, 5 }, 4, '#');

	// Выводим изображение в строковый поток
	std::ostringstream output;
	ImageService::Print(image, output);

	// Ожидаемый результат для правильной окружности радиуса 4
	std::string expected =
		"           \n"
		"    ###    \n"
		"   #   #   \n"
		"  #     #  \n"
		" #       # \n"
		" #       # \n"
		" #       # \n"
		"  #     #  \n"
		"   #   #   \n"
		"    ###    \n"
		"           \n";

	EXPECT_EQ(expected, output.str());
}