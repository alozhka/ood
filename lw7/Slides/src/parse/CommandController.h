#pragma once
#include <fstream>
#include <iomanip>

#include "Menu.h"

#include <istream>

#include "../canvas/SVGCanvas.h"
#include "../shape/factory/ShapesFactory.h"
#include "../slide/ISlide.h"
#include "../slide/Slide.h"

class CommandController
{
public:
	CommandController(std::istream& input, std::ostream& output)
		: m_menu{ input, output }
		, m_input{ input }
		, m_output{ output }
	{
		m_menu.AddItem(
			"List",
			"Usage: List. Lists shapes",
			[this](std::istream&) { List(); });
		m_menu.AddItem(
			"InsertShape",
			"Usage: InsertShape <shapeType> <fillColor> <lineColor> <x> <y> <width> <height>. Inserts shape into slide",
			std::bind_front(&CommandController::InsertShape, this));
		m_menu.AddItem(
			"TransformShape",
			"Usage: TransformShape <index> <left> <top> <width> <height>. Transforms shape's frame",
			std::bind_front(&CommandController::TransformShape, this));
		m_menu.AddItem(
			"GroupShapes",
			"Usage: GroupShapes <index1> <index2> <...>. Groups shapes into single group",
			std::bind_front(&CommandController::GroupShapes, this));
		m_menu.AddItem(
			"SetLineStyle",
			"Usage: SetLineStyle <index> <color> <enabled|disabled>. Sets line style color for shape",
			std::bind_front(&CommandController::SetLineStyle, this));
		m_menu.AddItem(
			"SetFillStyle",
			"Usage: SetFillStyle <index> <color> <enabled|disabled>. Sets fill style color for shape",
			std::bind_front(&CommandController::SetFillStyle, this));
		m_menu.AddItem(
			"CloneShape",
			"Usage: CloneShape <index>. Clones shape at specified index",
			std::bind_front(&CommandController::CloneShape, this));
		m_menu.AddItem(
			"Export",
			"Usage: Export <filename>. Export image in SVG",
			std::bind_front(&CommandController::Export, this));
		m_menu.AddItem(
			"Help",
			"Prints command list.",
			[this](std::istream&) { Help(); });
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
	void InsertShape(std::istream& input)
	{
		std::string type;
		if (!(input >> type))
		{
			throw std::invalid_argument("Shape type is not specified");
		}

		std::shared_ptr<Shape> shape = ShapesFactory::Create(type, input);
		m_slide->AddShape(shape);
	}

	void TransformShape(std::istream& input)
	{
		int index;
		double left, top, width, height;

		if (!(input >> index >> left >> top >> width >> height))
		{
			throw std::invalid_argument("Not all args are specified");
		}

		m_slide->TransformShape(--index, left, top, width, height);
	}

	void GroupShapes(std::istream& input)
	{
		std::set<int> indexes;
		int index;
		while (input >> index)
		{
			indexes.insert(--index);
		}

		m_slide->GroupShapes(indexes);
	}

	void SetLineStyle(std::istream& input)
	{
		int index;
		RGBAColor color;
		std::string enabledStr;

		if (!(input >> index >> std::hex >> color >> enabledStr))
		{
			throw std::invalid_argument("Not all args are specified");
		}

		bool isEnabled = ParseEnabledFlag(enabledStr);
		m_slide->SetLineStyle(--index, color, isEnabled);
	}

	void SetFillStyle(std::istream& input)
	{
		int index;
		RGBAColor color;
		std::string enabledStr;

		if (!(input >> index >> std::hex >> color >> enabledStr))
		{
			throw std::invalid_argument("Not all args are specified");
		}

		bool isEnabled = ParseEnabledFlag(enabledStr);
		m_slide->SetFillStyle(--index, color, isEnabled);
	}

	void CloneShape(std::istream& input)
	{
		int index;

		if (!(input >> index))
		{
			throw std::invalid_argument("Shape index is not specified");
		}

		m_slide->CloneShape(--index);
	}

	void Export(std::istream& input)
	{
		std::string filename;
		if (!(input >> filename))
		{
			throw std::invalid_argument("Filename is not specified");
		}

		SVGCanvas canvas{ 1920, 1200 };
		m_slide->Draw(canvas);

		std::ofstream file(filename);
		if (!file)
		{
			throw std::runtime_error("Cannot open file: " + filename);
		}

		canvas.Export(file);
	}

	void List() const
	{
		for (size_t i = 0; i < m_slide->GetShapesCount(); ++i)
		{
			std::shared_ptr<IShape> shape = m_slide->GetShapeAt(i);
			std::optional<Frame> frame = shape->GetFrame();
			m_output << std::format(
				"{}. Type: {}; Color: outline {}, inline {}; Frame: {}\n",
				i + 1,
				shape->GetType(),
				StyleToString(shape->GetLineStyle()),
				StyleToString(shape->GetFillStyle()),
				FrameToString(frame));
		}
	}

	void Help() const
	{
		m_menu.ShowInstructions();
	}

	void Exit()
	{
		m_menu.Exit();
	}

	static bool ParseEnabledFlag(const std::string& str)
	{
		if (str == "enabled")
		{
			return true;
		}
		if (str == "disabled")
		{
			return false;
		}
		throw std::invalid_argument("Invalid enabled flag");
	}

	static std::string StyleToString(std::optional<Style> style)
	{
		std::ostringstream ss;

		if (!style)
		{
			ss << "none";
		}
		else
		{
			ss << '#' << std::setw(8) << std::setfill('0') << std::hex << style->GetColor()
			   << " " << PrintEnabled(style->IsEnabled());
		}

		return ss.str();
	}

	static std::string PrintEnabled(bool isEnabled)
	{
		return isEnabled ? "enabled" : "disabled";
	}

	static std::string FrameToString(std::optional<Frame> frame)
	{
		if (!frame)
		{
			return "none";
		}

		return std::format(
			"left: {}, top: {}, width: {}, height: {}",
			frame->left,
			frame->top,
			frame->width,
			frame->height);
	}

	Menu m_menu;
	std::unique_ptr<ISlide> m_slide{ std::make_unique<Slide>() };

	std::istream& m_input;
	std::ostream& m_output;
};
