#include "lib/Duck/DecoyDuck.h"
#include "lib/Duck/MallardDuck.h"
#include "lib/Duck/ModelDuck.h"
#include "lib/Duck/RedheadDuck.h"
#include "lib/Duck/RubberDuck.h"
#include "lib/DuckFunctions.h"
#include <cstdlib>

int main()
{
	MallardDuck mallardDuck;
	PlayWithDuck(mallardDuck);

	RedheadDuck redheadDuck;
	PlayWithDuck(redheadDuck);

	RubberDuck rubberDuck;
	PlayWithDuck(rubberDuck);

	DecoyDuck decoyDuck;
	PlayWithDuck(decoyDuck);

	ModelDuck modelDuck;
	PlayWithDuck(modelDuck);

	modelDuck.SetFlyBehavior(std::make_unique<FlyWithWings>());
	PlayWithDuck(modelDuck);

	// Демонстрация нового функционала: кряканье после каждого второго полёта
	std::cout << "=== Демонстрация кряканья от удовольствия после каждого второго полёта ===" << std::endl;
	
	MallardDuck pleasureDuck;
	std::cout << "\nУтка кряква демонстрирует полёты с удовольствием:" << std::endl;
	pleasureDuck.Display();
	
	std::cout << "Полёт 1: ";
	pleasureDuck.FlyWithPleasure();
	
	std::cout << "Полёт 2: ";
	pleasureDuck.FlyWithPleasure();
	
	std::cout << "Полёт 3: ";
	pleasureDuck.FlyWithPleasure();
	
	std::cout << "Полёт 4: ";
	pleasureDuck.FlyWithPleasure();

	// Демонстрация с нелетающей уткой
	std::cout << "\nРезиновая утка (не умеет летать) пытается летать с удовольствием:" << std::endl;
	RubberDuck rubberPleasureDuck;
	rubberPleasureDuck.Display();
	
	std::cout << "Попытка полёта 1: ";
	rubberPleasureDuck.FlyWithPleasure();
	
	std::cout << "Попытка полёта 2: ";
	rubberPleasureDuck.FlyWithPleasure();

	return EXIT_SUCCESS;
}