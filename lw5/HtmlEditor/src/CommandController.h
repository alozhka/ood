#pragma once
#include "Menu.h"

class CommandController
{
public:
	explicit CommandController(std::istream& input, std::ostream& output)
		: m_menu{ input, output }
	{
		m_menu.AddItem(
			"Help",
			"Shows the available commands and required args",
			std::bind_front(&CommandController::PrintHelp, this));
		m_menu.AddItem(
			"Exit",
			"Usage: Exit. Exits the program.",
			std::bind_front(&CommandController::Exit, this));
	}

	void Run()
	{
		m_menu.Run();
	}

private:
	void PrintHelp(std::istream&)
	{
		m_menu.ShowInstructions();
	}

	void Exit(std::istream&)
	{
		m_menu.Exit();
	}

	Menu m_menu;
};