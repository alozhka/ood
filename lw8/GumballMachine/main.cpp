#include "src/GumballMachine.h"

#include <iostream>

template <typename GumballMachineType>
void TestGumballMachine(GumballMachineType& m)
{
	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.TurnCrank();

	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.EjectQuarter();
	m.TurnCrank();

	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();
	m.EjectQuarter();

	std::cout << m.ToString() << std::endl;

	m.InsertQuarter();
	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();
	m.InsertQuarter();
	m.TurnCrank();

	std::cout << m.ToString() << std::endl;
}

int main()
{
	GumballMachine gumball(2, std::cout);
	TestGumballMachine(gumball);

	return 0;
}
