#include "SFML/Graphics/RenderWindow.hpp"
#include "src/commands/ShapesController.h"
#include "src/draw/SFMLCanvas.h"
#include "src/shapes/Picture.h"

#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Shapes");
	shapes::Picture picture;
	gfx::SFMLCanvas canvas(window);
	ShapesController controller(picture, canvas, std::cin, std::cout);
	window.clear(sf::Color::Black);

	while (window.isOpen())
	{
		while (std::optional<sf::Event> event = window.pollEvent())
		{
			if (event.has_value() && event->is<sf::Event::Closed>())
			{
				window.close();
				return EXIT_SUCCESS;
			}
		}

		controller.ProcessCommand();

		window.display();

		sf::sleep(sf::milliseconds(15));
	}

	return EXIT_SUCCESS;
}
