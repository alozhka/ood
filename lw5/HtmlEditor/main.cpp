#include "src/CommandController.h"

#include <iostream>

int main()
{
	CommandController controller{ std::cin, std::cout };
	controller.Run();
	return 0;
}
