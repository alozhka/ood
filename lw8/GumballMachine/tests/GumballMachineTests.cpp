#include "../src/GumballMachine.h"

#include "gtest/gtest.h"
class GumballMachineTests : public testing::Test
{
protected:
	std::ostringstream output{};
	GumballMachine gumballMachine{ 4, output };
};

TEST_F(GumballMachineTests, NoQuarterState)
{
	output << gumballMachine.ToString() << std::endl;
	gumballMachine.TurnCrank();
	gumballMachine.EjectQuarter();
	gumballMachine.InsertQuarter();
	output << gumballMachine.ToString() << std::endl;

	EXPECT_EQ(
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 4 gumballs\n"
		"Machine is waiting for quarter\n"
		"You turned but there's no quarter\n"
		"You need to pay first\n"
		"You haven't inserted a quarter\n"
		"You inserted a quarter\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 4 gumballs\n"
		"Machine is waiting for turn of crank\n",
		output.str());
}
