#pragma once
#include "Document/Document.h"
#include "Document/IDocument.h"
#include "Menu.h"

class CommandController
{
public:
	explicit CommandController(std::istream& input, std::ostream& output)
		: m_document{ std::make_unique<Document>() }
		, m_menu{ input, output }
		, m_output{ output }
	{
		m_menu.AddItem(
			"Help",
			"Shows the available commands and required args",
			[this](std::istream&) { PrintHelp(); });
		m_menu.AddItem(
			"Exit",
			"Exits the program.",
			[this](std::istream&) { Exit(); });
		m_menu.AddItem(
			"Save",
			"Usage: Save <path>. Saves the file.",
			std::bind_front(&CommandController::Save, this));
		m_menu.AddItem(
			"SetTitle",
			"Usage: SetTitle <title>. Sets the document title.",
			std::bind_front(&CommandController::SetTitle, this));
		m_menu.AddItem(
			"List",
			"Shows the document title and items.",
			[this](std::istream&) { List(); });
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

	void Save(std::istream& input)
	{
		std::string path;
		if (!(input >> path))
		{
			throw std::runtime_error("Path is not specified");
		}

		m_document->Save(path);
	}

	void SetTitle(std::istream& input)
	{
		std::string begin, end;

		if (input >> begin)
		{
			std::getline(input, end);
		}
		std::string title = begin + end;

		m_document->SetTitle(title);
	}

	void List() const
	{
		m_output << "Title: " << m_document->GetTitle() << "\n";
	}

	std::unique_ptr<IDocument> m_document;
	Menu m_menu;
	std::ostream& m_output;
};