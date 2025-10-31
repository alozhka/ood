#include "src/Menu.h"
#include "src/Robot.h"

#include <iostream>

void BeginMacroCommand(Menu& menu)
{
	std::string shortcut, description;
	std::vector<std::string> commands;

	std::cout << "Command name: ";
	do
	{
		std::getline(std::cin, shortcut);
	} while (menu.CommandExists(shortcut) && std::cout << "Command already exists!\n");

	std::cout << "Command description: ";
	std::getline(std::cin, description);

	std::cout << "Write your commands. Type `end_macro` to finish recording a macro command\n";

	std::string command;
	while (std::cout << ">"
		&& std::getline(std::cin, command)
		&& command != "end_macro")
	{
		if (menu.CommandExists(command))
		{
			commands.push_back(command);
		}
		else
		{
			std::cout << "Command does not exist!\n";
		}
	}

	menu.AddMacroCommand(shortcut, description, commands);
	std::cout << "Macro command is saved\n";
}

void AddCommands(Menu& menu, Robot& robot)
{
	menu.AddItem("on", "Turns the Robot on", [&] {
		robot.TurnOn();
	});
	menu.AddItem("off", "Turns the Robot off",
		std::bind_front(&Robot::TurnOff, &robot));
	menu.AddItem("north", "Makes the Robot walk north",
		std::bind_front(&Robot::Walk, &robot, WalkDirection::North));
	menu.AddItem("south", "Makes the Robot walk south",
		std::bind_front(&Robot::Walk, &robot, WalkDirection::South));
	menu.AddItem("west", "Makes the Robot walk west",
		std::bind_front(&Robot::Walk, &robot, WalkDirection::West));
	menu.AddItem("east", "Makes the Robot walk east",
		std::bind_front(&Robot::Walk, &robot, WalkDirection::East));
	menu.AddItem("stop", "Stops the Robot",
		std::bind_front(&Robot::Stop, &robot));
	menu.AddItem("begin_macro", "Begins recording macro command", [&] {
		BeginMacroCommand(menu);
	});
	menu.AddItem("help", "Show instructions",
		std::bind_front(&Menu::ShowInstructions, &menu));
	menu.AddItem("exit", "Exit from this menu",
		std::bind_front(&Menu::Exit, &menu));
}

int main()
{
	Menu menu;
	Robot robot;
	AddCommands(menu, robot);

	menu.Run();

	return 0;
}
