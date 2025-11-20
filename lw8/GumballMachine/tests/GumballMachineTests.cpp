#include "../src/GumballMachine.h"

#include "gtest/gtest.h"
class GumballMachineTests : public testing::Test
{
protected:
	std::ostringstream output{};
	GumballMachine gumballMachine{ 2, output };
};

TEST_F(GumballMachineTests, CanInsertQuarter)
{
	output << gumballMachine.ToString() << std::endl;
	gumballMachine.TurnCrank();
	gumballMachine.EjectQuarter();
	gumballMachine.InsertQuarter();
	output << gumballMachine.ToString() << std::endl;

	EXPECT_EQ(
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 2 gumballs\n"
		"Machine is waiting for quarter\n"
		"You turned but there's no quarter\n"
		"You need to pay first\n"
		"You haven't inserted a quarter\n"
		"You inserted a quarter\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 2 gumballs\n"
		"Machine is waiting for turn of crank\n",
		output.str());
}

TEST_F(GumballMachineTests, ReceivesGumball)
{
	gumballMachine.InsertQuarter();
	output << gumballMachine.ToString() << std::endl;
	gumballMachine.TurnCrank();
	output << gumballMachine.ToString() << std::endl;

	EXPECT_EQ(
		"You inserted a quarter\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 2 gumballs\n"
		"Machine is waiting for turn of crank\n"
		"You turned...\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 1 gumball\n"
		"Machine is waiting for quarter\n",
		output.str());
}