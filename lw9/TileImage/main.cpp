#include <iostream>

#include "src/drawer/Drawer.h"
#include "src/image/ImageService.h"

#include <fstream>

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

	Image img{ { 30, 20 }, '.' };
	{
		Drawer::DrawLine(img, { 3, 2 }, { 26, 5 }, 0xFFFFFF);
		Drawer::DrawLine(img, { 26, 5 }, { 21, 18 }, 0xFFFFFF);
		Drawer::DrawLine(img, { 21, 18 }, { 3, 2 }, 0xFFFFFF);
	}

	{
		Image img1{ img };
		Drawer::DrawCircle(img, { 15, 15 }, 4, '@');
	}

	{
		Image img2{ img };
		Drawer::FillCircle(img, { 5, 5 }, 4, '$');
	}
	ImageService::Print(img, std::cout);

	std::ofstream out("image.ppm");
	ImageService::SaveToPPM(img, out);
}
