#pragma once
#include "Menu.h"
#include "MultiGumballMachine.h"

class CommandController
{
public:
	explicit CommandController(MultiGumballMachine& gumballMachine, std::istream& input, std::ostream& output)
		: m_menu(input, output)
		, m_gumballMachine(gumballMachine)
		, m_output(output)
	{
		m_menu.AddItem(
			"help",
			"Usage: help. Prints commands",
			[this](std::istream&) { PrintHelp(); });
		m_menu.AddItem(
			"insert",
			"Usage: insert. Insert a quarter into the machine",
			[this](std::istream&) { InsertQuarter(); });
		m_menu.AddItem(
			"turn",
			"Usage: turn. Turn the crank to get a gumball",
			[this](std::istream&) { TurnCrank(); });
		m_menu.AddItem(
			"return",
			"Usage: return. Return all inserted quarters",
			[this](std::istream&) { ReturnQuarters(); });
		m_menu.AddItem(
			"refill",
			"Usage: refill <count>. Refill machine with specified number of gumballs",
			std::bind_front(&CommandController::Refill, this));
		m_menu.AddItem(
			"info",
			"Usage: info. Show current machine state",
			[this](std::istream&) { ShowInfo(); });
		m_menu.AddItem(
			"exit",
			"Usage: exit. Exit the program",
			[this](std::istream&) { Exit(); });
	}

	void Run()
	{
		m_menu.Run();
	}

private:
	void PrintHelp() const
	{
		m_menu.ShowInstructions();
	}

	void InsertQuarter()
	{
		m_gumballMachine.InsertQuarter();
	}

	void TurnCrank()
	{
		m_gumballMachine.TurnCrank();
	}

	void ReturnQuarters()
	{
		m_gumballMachine.EjectQuarter();
	}

	void Refill(std::istream& in)
	{
		unsigned count;
		if (in >> count)
		{
			m_gumballMachine.Refill(count);
		}
		else
		{
			throw std::runtime_error("Invalid refill count. Usage: refill <count>");
		}
	}

	void ShowInfo() const
	{
		m_output << m_gumballMachine.ToString() << std::endl;
	}

	void Exit()
	{
		m_menu.Exit();
	}

	Menu m_menu;
	MultiGumballMachine& m_gumballMachine;
	std::ostream& m_output;
};