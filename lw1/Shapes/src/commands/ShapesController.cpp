#include "ShapesController.h"

#include "ShapesFactory.h"

#include <sstream>

ShapesController::ShapesController(shapes::Picture& picture, gfx::ICanvas& canvas, std::istream& input, std::ostream& output)
	: m_actions{
		{ "AddShape", std::bind_front(&ShapesController::ProcessAddShape, this) },
		{ "DrawPicture", std::bind_front(&ShapesController::ProcessDrawPicture, this) }
	}
	, m_picture(picture)
	, m_canvas(canvas)
	, m_input(input)
	, m_output(output)
{
	StartInputThread();
}

void ShapesController::ProcessCommand()
{
	std::string command;
	{
		std::lock_guard lock(m_queueMutex);
		if (m_commandQueue.empty())
		{
			return;
		}
		command = m_commandQueue.front();
		m_commandQueue.pop();
	}

	std::istringstream iss(command);
	std::string action;
	iss >> action;

	const auto it = m_actions.find(action);
	if (it == m_actions.end())
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

void ShapesController::StartInputThread()
{
	m_inputThread = std::thread(&ShapesController::InputThreadFunction, this);
}

void ShapesController::StopInputThread()
{
	if (m_inputThread.joinable())
	{
		m_threadStopped = true;
		m_inputThread.join();
	}
}

ShapesController::~ShapesController()
{
	StopInputThread();
}

void ShapesController::ProcessAddShape(std::istream& input)
{
	std::string id, color, type;
	if (!(input >> id >> color >> type))
	{
		throw std::invalid_argument("Invalid arguments. Usage: AddShape <id> <цвет> <тип> <параметры>");
	}

	std::unique_ptr<shapes::IShapeStrategy> strategy = ShapesFactory::CreateFromStream(type, input);
	auto shape = std::make_unique<Shape>(id, gfx::Color::FromHex(color), std::move(strategy));
	m_picture.AddShape(std::move(shape));
}

void ShapesController::ProcessDrawPicture(std::istream&)
{
	m_picture.Draw(m_canvas);
}

void ShapesController::InputThreadFunction()
{
	std::string line;
	while (!m_threadStopped && std::getline(m_input, line))
	{
		std::lock_guard lock(m_queueMutex);
		m_commandQueue.emplace(line);
		return;
	}
}