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
			[this](std::istream&) { PrintHelp(); });
		m_menu.AddItem(
			"Exit",
			"Exits the program.",
			[this](std::istream&) { Exit(); });
	}

	void Run()
	{
		m_menu.Run();
	}

private:
	void PrintHelp()
	{
		m_menu.ShowInstructions();
	}

	void Exit()
	{
		m_menu.Exit();
	}

	Menu m_menu;
};