#include "CommandController.h"

#include "ShapesFactory.h"

#include <sstream>

CommandController::CommandController(gfx::ICanvas& canvas, std::istream& input, std::ostream& output)
	: m_canvas(canvas)
	, m_input(input)
	, m_output(output)
	, m_actions{ { "AddShape", std::bind_front(&CommandController::ProcessAddShape, this) } }
{
}

void CommandController::ProcessCommand()
{
	std::string command;
	std::getline(m_input, command);

	std::istringstream iss(command);
	std::string action;
	iss >> action;

	const auto it = m_actions.find(action);
	if (it != m_actions.end())
	{
		m_output << "Invalid command\n";
		return;
	}

	try
	{
		it->second(iss);
	}
	catch (const std::exception& e)
	{
		m_output << e.what() << std::endl;
	}
}

void CommandController::ProcessAddShape(std::istream& input)
{
	std::string id, color, type;
	if (!(input >> id >> color >> type))
	{
		throw std::invalid_argument("Invalid arguments. Usage: AddShape <id> <цвет> <тип> <параметры>");
	}

	IShapeStrategy shape = ShapesFactory::CreateFromStream(type, input);
}