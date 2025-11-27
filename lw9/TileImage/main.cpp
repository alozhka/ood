#include <iostream>

#include "src/drawer/Drawer.h"
#include "src/image/ImageService.h"

constexpr int STAR_COUNT = 20;

int main()
{
	{
		Image img = ImageService::LoadImage(
			" CCCC             \n"
			"CC  CC   ##    ## \n"
			"CC      ####  ####\n"
			"CC  CC   ##    ## \n"
			" CCCC             \n");
		ImageService::Print(img, std::cout);
	}

	{
		Image img{ { 30, 20 }, '.' };

		Drawer::DrawLine(img, { 3, 2 }, { 26, 5 }, '#');
		Drawer::DrawLine(img, { 26, 5 }, { 21, 18 }, '#');
		Drawer::DrawLine(img, { 21, 18 }, { 3, 2 }, '#');

		ImageService::Print(img, std::cout);
	}

	{
		Image img{ { 11, 11 }, ' ' };

		// Рисуем окружность радиуса 4 с центром в (5, 5)
		Drawer::DrawCircle(img, { 5, 5 }, 4, '@');

		ImageService::Print(img, std::cout);
	}
}
