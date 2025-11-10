#include <iostream>

#include "src/parse/CommandController.h"

int main()
{
    CommandController controller(std::cin, std::cout);
    controller.Run();

    return 0;
}
