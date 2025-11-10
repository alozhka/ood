#pragma once
#include <functional>
#include <istream>
#include <ostream>
#include <sstream>
#include <vector>

class Menu
{
public:
	using Command = std::function<void(std::istream&)>;

	Menu(std::istream& input, std::ostream& output)
		: m_input(input)
		, m_output(output)
	{
	}

	void AddItem(
		const std::string& shortcut,
		const std::string& description,
		const Command& command)
	{
		m_items.emplace_back(shortcut, description, command);
	}

	void Run()
	{
		std::string commandLine;
		while (!m_exit && getline(m_input, commandLine))
		{
			try
			{
				ExecuteCommand(commandLine);
			}
			catch (std::exception& e)
			{
				m_output << e.what() << std::endl;
			}
		}
	}

	void ShowInstructions() const
	{
		m_output << "Commands list:\n";
		for (auto& item : m_items)
		{
			m_output << "  " << item.shortcut << ": " << item.description << "\n";
		}
	}

	void Exit()
	{
		m_exit = true;
	}

private:
	void ExecuteCommand(const std::string& line)
	{
		std::istringstream iss(line);
		std::string commandName;
		iss >> commandName;

		const auto it = std::ranges::find_if(m_items, [&](const Item& item) {
			return item.shortcut == commandName;
		});

		if (it != m_items.end())
		{
			it->command(iss);
		}
		else
		{
			m_output << "Unknown command\n";
		}
	}

	struct Item
	{
		Item(const std::string& shortcut, const std::string& description, const Command& factory)
			: shortcut(shortcut)
			, description(description)
			, command(factory)
		{
		}

		std::string shortcut;
		std::string description;
		Command command;
	};

	std::vector<Item> m_items;
	bool m_exit = false;
	std::istream& m_input;
	std::ostream& m_output;
};
