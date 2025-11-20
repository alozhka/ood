#include "../src/MultiGumballMachine.h"
#include "../src/NaiveMultiGumballMachine.h"
#include "gtest/gtest.h"
#include <sstream>

class GumballMachineTests : public testing::Test
{
protected:
	void ClearOutputs()
	{
		naiveOutput.str("");
		stateOutput.str("");
	}

	std::ostringstream naiveOutput{}, stateOutput{};
	NaiveMultiGumballMachine naiveMachine{ 3, naiveOutput };
	MultiGumballMachine stateMachine{ 3, stateOutput };
};

TEST_F(GumballMachineTests, InitialState)
{
	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();

	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(GumballMachineTests, InsertSingleQuarter)
{
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("You inserted a quarter\n", naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(GumballMachineTests, InsertMultipleQuarters)
{
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	ClearOutputs();

	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("You inserted another quarter\n", naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 2/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(GumballMachineTests, HasMaxQuarterLimit)
{
	for (int i = 0; i < 5; ++i)
	{
		naiveMachine.InsertQuarter();
		stateMachine.InsertQuarter();
	}

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 5/5\n"
		"Machine is waiting for turn of crank");

	ClearOutputs();
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("You can't insert another quarter, the machine is full\n", naiveOutput.str());
}

TEST_F(GumballMachineTests, ReturnsLeftMoney)
{
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	ClearOutputs();
	naiveMachine.EjectQuarter();
	stateMachine.EjectQuarter();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("Returning 2 quarters\n", naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(GumballMachineTests, TurnCrankWithQuarters)
{
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	ClearOutputs();
	naiveMachine.TurnCrank();
	stateMachine.TurnCrank();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ(
		"You turned...\n"
		"A gumball comes rolling out the slot...\n",
		naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 2 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(GumballMachineTests, MultipleTurnsWithMultipleQuarters)
{
	naiveMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	ClearOutputs();

	naiveMachine.TurnCrank();
	stateMachine.TurnCrank();
	naiveMachine.TurnCrank();
	stateMachine.TurnCrank();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ(
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n",
		naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 1 gumball\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(GumballMachineTests, MoreQuartersThanGumballs)
{
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	ClearOutputs();
	naiveMachine.TurnCrank();
	naiveMachine.TurnCrank();
	naiveMachine.TurnCrank();
	stateMachine.TurnCrank();
	stateMachine.TurnCrank();
	stateMachine.TurnCrank();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ(
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"Oops, out of gumballs\n",
		naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 0 gumballs\n"
		"Quarters inserted: 1/5\n"
		"Machine is sold out");

	ClearOutputs();
	naiveMachine.EjectQuarter();
	stateMachine.EjectQuarter();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("Returning 1 quarter\n", naiveOutput.str());
}

TEST_F(GumballMachineTests, SoldOutState)
{
	std::ostringstream naiveEmptyOutput, stateEmptyOutput;
	NaiveMultiGumballMachine naiveEmpty(0, naiveEmptyOutput);
	MultiGumballMachine stateEmpty(0, stateEmptyOutput);
	EXPECT_EQ(naiveEmpty.ToString(), stateEmpty.ToString());
	EXPECT_EQ(naiveEmpty.ToString(),
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 0 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is sold out");

	naiveEmptyOutput.str("");
	stateEmptyOutput.str("");
	naiveEmpty.InsertQuarter();
	stateEmpty.InsertQuarter();

	EXPECT_EQ(naiveEmptyOutput.str(), stateEmptyOutput.str());
	EXPECT_EQ(naiveEmptyOutput.str(), "You can't insert a quarter, the machine is sold out\n");
}

TEST_F(GumballMachineTests, EjectFromSoldOutWithQuarters)
{
	std::ostringstream naiveSpecialOutput, stateSpecialOutput;
	NaiveMultiGumballMachine naiveSpecial(1, naiveSpecialOutput);
	MultiGumballMachine stateSpecial(1, stateSpecialOutput);

	naiveSpecial.InsertQuarter();
	stateSpecial.InsertQuarter();
	naiveSpecial.InsertQuarter();
	stateSpecial.InsertQuarter();
	naiveSpecial.InsertQuarter();
	stateSpecial.InsertQuarter();
	naiveSpecial.TurnCrank();
	stateSpecial.TurnCrank();

	std::string naiveString = naiveSpecial.ToString();
	std::string stateString = stateSpecial.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 0 gumballs\n"
		"Quarters inserted: 2/5\n"
		"Machine is sold out");

	naiveSpecialOutput.str("");
	stateSpecialOutput.str("");
	naiveSpecial.EjectQuarter();
	stateSpecial.EjectQuarter();

	EXPECT_EQ(naiveSpecialOutput.str(), stateSpecialOutput.str());
	EXPECT_EQ(naiveSpecialOutput.str(), "Returning 2 quarters\n");
}

TEST_F(GumballMachineTests, TurnCrankWithoutQuarters)
{
	ClearOutputs();
	naiveMachine.TurnCrank();
	stateMachine.TurnCrank();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("You turned but there's no quarter\nYou need to pay first\n", naiveOutput.str());
}

TEST_F(GumballMachineTests, EjectWithoutQuarters)
{
	ClearOutputs();
	naiveMachine.EjectQuarter();
	stateMachine.EjectQuarter();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("You haven't inserted a quarter\n", naiveOutput.str());
}

TEST_F(GumballMachineTests, ComplexUsageScenarioFullOutput)
{
	naiveMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	stateMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ(
		"You inserted a quarter\n"
		"You inserted another quarter\n"
		"You inserted another quarter\n",
		naiveOutput.str());

	ClearOutputs();

	naiveMachine.TurnCrank();
	naiveMachine.TurnCrank();
	stateMachine.TurnCrank();
	stateMachine.TurnCrank();

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ(
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n",
		naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 1 gumball\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(GumballMachineTests, RefillInSoldOutState)
{
	std::ostringstream naiveEmptyOutput, stateEmptyOutput;
	NaiveMultiGumballMachine naiveEmpty(0, naiveEmptyOutput);
	MultiGumballMachine stateEmpty(0, stateEmptyOutput);

	naiveEmptyOutput.str("");
	stateEmptyOutput.str("");
	naiveEmpty.Refill(5);
	stateEmpty.Refill(5);

	EXPECT_EQ(naiveEmptyOutput.str(), stateEmptyOutput.str());
	EXPECT_EQ("Machine refilled with 5 gumballs\n", naiveEmptyOutput.str());

	std::string naiveString = naiveEmpty.ToString();
	std::string stateString = stateEmpty.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 5 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(GumballMachineTests, RefillInNoQuarterState)
{
	ClearOutputs();
	naiveMachine.Refill(10);
	stateMachine.Refill(10);

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("Machine refilled with 10 gumballs\n", naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 10 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(GumballMachineTests, RefillInHasQuarterState)
{
	naiveMachine.InsertQuarter();
	stateMachine.InsertQuarter();

	ClearOutputs();
	naiveMachine.Refill(7);
	stateMachine.Refill(7);

	EXPECT_EQ(naiveOutput.str(), stateOutput.str());
	EXPECT_EQ("Machine refilled with 7 gumballs\n", naiveOutput.str());

	std::string naiveString = naiveMachine.ToString();
	std::string stateString = stateMachine.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 7 gumballs\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(GumballMachineTests, RefillInSoldOutStateWithQuarters)
{
	std::ostringstream naiveEmptyOutput, stateEmptyOutput;
	NaiveMultiGumballMachine naiveEmpty(1, naiveEmptyOutput);
	MultiGumballMachine stateEmpty(1, stateEmptyOutput);

	naiveEmpty.InsertQuarter();
	stateEmpty.InsertQuarter();
	naiveEmpty.InsertQuarter();
	stateEmpty.InsertQuarter();
	naiveEmpty.TurnCrank();
	stateEmpty.TurnCrank();

	std::string beforeRefill = naiveEmpty.ToString();
	EXPECT_EQ(beforeRefill,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 0 gumballs\n"
		"Quarters inserted: 1/5\n"
		"Machine is sold out");

	naiveEmptyOutput.str("");
	stateEmptyOutput.str("");
	naiveEmpty.Refill(4);
	stateEmpty.Refill(4);

	EXPECT_EQ(naiveEmptyOutput.str(), stateEmptyOutput.str());
	EXPECT_EQ("Machine refilled with 4 gumballs\n", naiveEmptyOutput.str());

	std::string naiveString = naiveEmpty.ToString();
	std::string stateString = stateEmpty.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 4 gumballs\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}

