#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

class Menu
{
public:
	using Command = std::function<void()>;
	void AddItem(
		const std::string& shortcut,
		const std::string& description,
		const Command& command)
	{
		m_items.emplace_back(shortcut, description, command);
	}

	void AddMacroCommand(
		const std::string& shortcut,
		const std::string& description,
		const std::vector<std::string>& shortcuts)
	{
		std::vector<Command> commands;
		for (const std::string& command : shortcuts)
		{
			const auto it = std::ranges::find_if(m_items, [&](const Item& item) {
				return item.shortcut == command;
			});
			if (it != m_items.end())
			{
				commands.push_back(it->command);
			}
		}

		Command macroCommand = [commands] {
			for (const auto& command : commands)
			{
				command();
			}
		};
		m_items.emplace_back(shortcut, description, macroCommand);
	}

	void Run()
	{
		ShowInstructions();

		std::string command;
		while (std::cout << ">"
			&& getline(std::cin, command)
			&& ExecuteCommand(command))
		{
		}
	}

	void ShowInstructions() const
	{
		std::cout << "Commands list:\n";
		for (auto& item : m_items)
		{
			std::cout << "  " << item.shortcut << ": " << item.description << "\n";
		}
	}

	void Exit()
	{
		m_exit = true;
	}

	bool CommandExists(const std::string& shortcut) const
	{
		const auto it = std::ranges::find_if(m_items, [&](const Item& item) {
			return item.shortcut == shortcut;
		});
		return it != m_items.end();
	}

private:
	bool ExecuteCommand(const std::string& command)
	{
		m_exit = false;
		auto it = std::ranges::find_if(m_items, [&](const Item& item) {
			return item.shortcut == command;
		});
		if (it != m_items.end())
		{
			it->command();
		}
		else
		{
			std::cout << "Unknown command\n";
		}
		return !m_exit;
	}

	struct Item
	{
		Item(const std::string& shortcut, const std::string& description, const Command& command)
			: shortcut(shortcut)
			, description(description)
			, command(command)
		{
		}

		std::string shortcut;
		std::string description;
		Command command;
	};
	std::vector<Item> m_items;
	bool m_exit = false;
};