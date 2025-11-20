#include "../src/MultiGumballMachine.h"
#include "../src/NaiveMultiGumballMachine.h"
#include "gtest/gtest.h"
#include <sstream>

// Сравнительные тесты - проверяем что обе реализации ведут себя одинаково
class MultiGumballMachineComparisonTests : public testing::Test
{
protected:
	void SetUp() override
	{
		naiveOutput = std::make_unique<std::ostringstream>();
		stateOutput = std::make_unique<std::ostringstream>();
		naiveMachine = std::make_unique<NaiveMultiGumballMachine>(3, *naiveOutput);
		stateMachine = std::make_unique<MultiGumballMachine>(3, *stateOutput);
	}

	void ClearOutputs()
	{
		naiveOutput->str("");
		stateOutput->str("");
	}

	std::unique_ptr<std::ostringstream> naiveOutput, stateOutput;
	std::unique_ptr<NaiveMultiGumballMachine> naiveMachine;
	std::unique_ptr<MultiGumballMachine> stateMachine;
};

TEST_F(MultiGumballMachineComparisonTests, InitialState)
{
	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();

	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(MultiGumballMachineComparisonTests, InsertSingleQuarter)
{
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(), "You inserted a quarter\n");

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(MultiGumballMachineComparisonTests, InsertMultipleQuarters)
{
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	ClearOutputs();

	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(), "You inserted another quarter\n");

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 2/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(MultiGumballMachineComparisonTests, HasMaxQuarterLimit)
{
	for (int i = 0; i < 5; ++i)
	{
		naiveMachine->InsertQuarter();
		stateMachine->InsertQuarter();
	}

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 5/5\n"
		"Machine is waiting for turn of crank");

	ClearOutputs();
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(), "You can't insert another quarter, the machine is full\n");
}

TEST_F(MultiGumballMachineComparisonTests, ReturnsLeftMoney)
{
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	ClearOutputs();
	naiveMachine->EjectQuarter();
	stateMachine->EjectQuarter();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(), "Returning 2 quarters\n");

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 3 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(MultiGumballMachineComparisonTests, TurnCrankWithQuarters)
{
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	ClearOutputs();
	naiveMachine->TurnCrank();
	stateMachine->TurnCrank();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(),
		"You turned...\n"
		"A gumball comes rolling out the slot...\n");

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 2 gumballs\n"
		"Quarters inserted: 0/5\n"
		"Machine is waiting for quarter");
}

TEST_F(MultiGumballMachineComparisonTests, MultipleTurnsWithMultipleQuarters)
{
	naiveMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	ClearOutputs();

	naiveMachine->TurnCrank();
	stateMachine->TurnCrank();
	naiveMachine->TurnCrank();
	stateMachine->TurnCrank();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(),
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n");

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 1 gumball\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}

TEST_F(MultiGumballMachineComparisonTests, MoreQuartersThanGumballs)
{
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	ClearOutputs();
	naiveMachine->TurnCrank();
	naiveMachine->TurnCrank();
	naiveMachine->TurnCrank();
	stateMachine->TurnCrank();
	stateMachine->TurnCrank();
	stateMachine->TurnCrank();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(),
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"Oops, out of gumballs\n");

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 0 gumballs\n"
		"Quarters inserted: 1/5\n"
		"Machine is sold out");

	ClearOutputs();
	naiveMachine->EjectQuarter();
	stateMachine->EjectQuarter();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(), "Returning 1 quarter\n");
}

TEST_F(MultiGumballMachineComparisonTests, SoldOutState)
{
	std::ostringstream naiveEmptyOutput, stateEmptyOutput;
	NaiveMultiGumballMachine naiveEmpty(0, naiveEmptyOutput);
	MultiGumballMachine stateEmpty(0, stateEmptyOutput);

	std::string naiveString = naiveEmpty.ToString();
	std::string stateString = stateEmpty.ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
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

TEST_F(MultiGumballMachineComparisonTests, EjectFromSoldOutWithQuarters)
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

TEST_F(MultiGumballMachineComparisonTests, TurnCrankWithoutQuarters)
{
	ClearOutputs();
	naiveMachine->TurnCrank();
	stateMachine->TurnCrank();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(), "You turned but there's no quarter\nYou need to pay first\n");
}

TEST_F(MultiGumballMachineComparisonTests, EjectWithoutQuarters)
{
	ClearOutputs();
	naiveMachine->EjectQuarter();
	stateMachine->EjectQuarter();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(), "You haven't inserted a quarter\n");
}

TEST_F(MultiGumballMachineComparisonTests, ComplexUsageScenarioFullOutput)
{
	naiveMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	naiveMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	stateMachine->InsertQuarter();
	stateMachine->InsertQuarter();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(),
		"You inserted a quarter\n"
		"You inserted another quarter\n"
		"You inserted another quarter\n");

	ClearOutputs();

	naiveMachine->TurnCrank();
	naiveMachine->TurnCrank();
	stateMachine->TurnCrank();
	stateMachine->TurnCrank();

	EXPECT_EQ(naiveOutput->str(), stateOutput->str());
	EXPECT_EQ(naiveOutput->str(),
		"You turned...\n"
		"A gumball comes rolling out the slot...\n"
		"You turned...\n"
		"A gumball comes rolling out the slot...\n");

	std::string naiveString = naiveMachine->ToString();
	std::string stateString = stateMachine->ToString();
	EXPECT_EQ(naiveString, stateString);
	EXPECT_EQ(naiveString,
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
		"Inventory: 1 gumball\n"
		"Quarters inserted: 1/5\n"
		"Machine is waiting for turn of crank");
}