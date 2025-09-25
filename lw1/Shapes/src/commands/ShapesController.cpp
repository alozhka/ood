#include "ShapesController.h"

#include "ShapesFactory.h"

#include <sstream>

ShapesController::ShapesController(shapes::Picture& picture, gfx::ICanvas& canvas, std::istream& input, std::ostream& output)
	: m_actions{
		{ "AddShape", std::bind_front(&ShapesController::ProcessAddShape, this) },
		{ "MoveShape", std::bind_front(&ShapesController::ProcessMoveShape, this) },
		{ "DrawShape", std::bind_front(&ShapesController::ProcessDrawShape, this) },
		{"DeleteShape", std::bind_front(&ShapesController::ProcessDeleteShape, this)},
		{ "MovePicture", std::bind_front(&ShapesController::ProcessMovePicture, this) },
		{ "DrawPicture", std::bind_front(&ShapesController::ProcessDrawPicture, this) },
		{ "List", std::bind_front(&ShapesController::ProcessList, this) }
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
	auto shape = std::make_unique<Shape>(id, gfx::Color::FromHexStr(color), std::move(strategy));
	m_picture.AddShape(std::move(shape));
}

void ShapesController::ProcessMoveShape(std::istream& input)
{
	std::string id;
	double x, y;
	if (!(input >> id >> x >> y))
	{
		throw std::runtime_error("Invalid arguments. Usage: MoveShape <id> <dx> <dy>");
	}

	m_picture.MoveShape(id, x, y);
}

void ShapesController::ProcessDeleteShape(std::istream& input)
{
	std::string id;
	if (!(input >> id))
	{
		throw std::runtime_error("Invalid arguments. Usage: DeleteShape <id>");
	}

	m_picture.RemoveShape(id);
}

void ShapesController::ProcessMovePicture(std::istream& input)
{
	double x, y;
	if (!(input >> x >> y))
	{
		throw std::runtime_error("Invalid arguments. Usage: MoveShape <dx> <dy>");
	}

	m_picture.MovePicture(x, y);
}

void ShapesController::ProcessList(std::istream&) const
{
	std::vector<Shape*> shapes = m_picture.ListShapes();
	unsigned int i = 1;
	for (const auto& shape : shapes)
	{
		m_output << i++ << " " << shape->GetDescription() << std::endl;
	}
}

void ShapesController::ProcessChangeColor(std::istream& input)
{
	std::string id, color;
	if (!(input >> id >> color))
	{
		throw std::runtime_error("Invalid arguments. Usage: ChangeColor <id> <color>");
	}

	m_picture.ChangeShapeColor(id, gfx::Color::FromHexStr(color));
}

void ShapesController::ProcessChangeShape(std::istream& input)
{
	std::string id, type;
	if (!(input >> id >> type))
	{
		throw std::runtime_error("Invalid arguments. Usage: ChangeShape <id> <type> <params>");
	}

	auto strategy = ShapesFactory::CreateFromStream(type, input);
	m_picture.ChangeShapeStrategy(id, std::move(strategy));
}

void ShapesController::ProcessDrawShape(std::istream& input)
{
	std::string id;
	if (!(input >> id))
	{
		throw std::invalid_argument("Invalid arguments. Usage: DrawShape <id>");
	}

	m_picture.DrawShape(id, m_canvas);
}

void ShapesController::ProcessDrawPicture(std::istream&)
{
	m_picture.DrawPicture(m_canvas);
}

void ShapesController::InputThreadFunction()
{
	std::string line;
	while (!m_threadStopped)
	{
		if (std::getline(m_input, line))
		{
			std::lock_guard lock(m_queueMutex);
			m_commandQueue.emplace(line);
		}
		else
		{
			break;
		}
	}
}