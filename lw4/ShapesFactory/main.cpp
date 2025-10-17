#include "src/canvas/SVGCanvas.h"
#include "src/people/Client.h"
#include "src/people/Designer.h"
#include "src/people/Painter.h"
#include "src/shapes/ShapeFactory.h"

#include <iostream>

//TODO: нарисовать нормальную картинку
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: ShapesFactory <output_file>\n";
		return EXIT_FAILURE;
	}

	SVGCanvas canvas;
	ShapeFactory shapeFactory;
	Designer designer(shapeFactory);
	Painter painter;
	Client client(designer);

	client.HandleCommand(std::cin, canvas, painter);

	canvas.SaveToFile(argv[1]);
	return EXIT_SUCCESS;
}
