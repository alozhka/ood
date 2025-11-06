#include "src/adapter/ModernGraphicsAdapter.h"
#include "src/modern_graphics_lib.h"
#include "src/shapes_drawing_lib.h"

// Пространство имен приложения (доступно для модификации)
namespace app
{
void PaintPicture(shape_drawing_lib::CanvasPainter& painter)
{
	using namespace shape_drawing_lib;
	Triangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 });
	Rectangle rectangle({ 30, 40 }, 18, 24);

	painter.Draw(triangle);
	painter.Draw(rectangle);
}

void PaintPictureOnCanvas()
{
	graphics_lib::Canvas simpleCanvas;
	shape_drawing_lib::CanvasPainter painter(simpleCanvas);
	PaintPicture(painter);
}

void PaintPictureOnModernGraphicsRenderer()
{
	modern_graphics_lib::ModernGraphicsRenderer renderer(std::cout);
	ModernGraphicsAdapter adapter{ renderer };
	shape_drawing_lib::CanvasPainter painter{ adapter };

	renderer.BeginDraw();
	PaintPicture(painter);
	renderer.EndDraw();
}
} // namespace app

int main()
{
	std::cout << "Should we use new API (y)?";
	std::string userInput;
	if (getline(std::cin, userInput) && (userInput == "y" || userInput == "Y"))
	{
		app::PaintPictureOnModernGraphicsRenderer();
	}
	else
	{
		app::PaintPictureOnCanvas();
	}
	return 0;
}