#include "src/CommandController.h"
#include "src/MultiGumballMachine.h"

#include <iostream>

int main()
{
	MultiGumballMachine gumball(2, std::cout);
	CommandController controller(gumball, std::cin, std::cout);

	controller.Run();

	return 0;
}
