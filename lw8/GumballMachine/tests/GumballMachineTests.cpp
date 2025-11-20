#include "../src/GumballMachine.h"
#include "TestGumballMachine.h"

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
		"You can't insert another quarter\n"
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
		"A gumball comes rolling out the slot...\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 1 gumball\n"
		"Machine is waiting for quarter\n",
		output.str());
}

TEST_F(GumballMachineTests, EjectsQuarter)
{
	gumballMachine.InsertQuarter();
	output << gumballMachine.ToString() << std::endl;
	gumballMachine.EjectQuarter();
	output << gumballMachine.ToString() << std::endl;

	EXPECT_EQ(
		"You inserted a quarter\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 2 gumballs\n"
		"Machine is waiting for turn of crank\n"
		"Quarter returned\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 2 gumballs\n"
		"Machine is waiting for quarter\n",
		output.str());
}

TEST_F(GumballMachineTests, CanBecomeSoldOut)
{
	gumballMachine.InsertQuarter();
	gumballMachine.TurnCrank();
	gumballMachine.InsertQuarter();
	gumballMachine.TurnCrank();
	gumballMachine.InsertQuarter();
	gumballMachine.TurnCrank();
	gumballMachine.EjectQuarter();
	output << gumballMachine.ToString() << std::endl;

	EXPECT_EQ(
		"You inserted a quarter\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You inserted a quarter\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"Oops, out of gumballs\n"
		"You can't insert a quarter, the machine is sold out\n"
		"You turned but there's no gumballs\n"
		"No gumball dispensed\n"
		"You can't eject, you haven't inserted a quarter yet\n"
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: 0 gumballs\n"
		"Machine is sold out\n",
		output.str());
}

TEST_F(GumballMachineTests, TestSoldOutState)
{
	TestGumballMachine gumballMachine{ 2, output };
	gumballMachine.SetState(gumballMachine.GetSoldState());

	gumballMachine.EjectQuarter();
	gumballMachine.InsertQuarter();
	gumballMachine.TurnCrank();
	output << gumballMachine.ToString() << std::endl;

	EXPECT_EQ(
		"Sorry you already turned the crank\n"
		"Please wait, we're already giving you a gumball\n"
		"Turning twice doesn't get you another gumball\n"
		"delivering a gumball\n",
		output.str());
}