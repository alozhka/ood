#pragma once
#include "Document/Document.h"
#include "Document/DocumentItem.h"
#include "Document/IDocument.h"
#include "Menu.h"

#include <filesystem>

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
		m_menu.AddItem(
			"InsertParagraph",
			"Usage: InsertParagraph <position>|end <text>. Inserts a paragraph.",
			std::bind_front(&CommandController::InsertParagraph, this));
		m_menu.AddItem(
			"ReplaceText",
			"Usage: ReplaceText <position> <text>. Replaces a paragraph with specified text.",
			std::bind_front(&CommandController::ReplateText, this));
	}

	void Run()
	{
		m_menu.Run();
	}

private:
	void PrintHelp() const
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
		std::string title = ReadText(input);
		m_document->SetTitle(title);
	}

	void List() const
	{
		m_output << "Title: " << m_document->GetTitle() << "\n";

		for (size_t i = 0; i < m_document->GetItemsCount(); ++i)
		{
			auto item = m_document->GetItem(i);
			m_output << (i + 1) << ". ";

			if (auto paragraph = item->GetParagraph())
			{
				m_output << "Paragraph: " << paragraph->GetText() << "\n";
			}
			// TODO: добавить поддержку изображений
		}
	}

	void InsertParagraph(std::istream& input)
	{
		std::string positionStr;
		if (!(input >> positionStr))
		{
			throw std::runtime_error("Position is not specified");
		}
		std::string text = ReadText(input);

		std::optional<size_t> position;
		if (positionStr != "end")
		{
			try
			{
				position = std::stoull(positionStr);
			}
			catch (...)
			{
				throw std::runtime_error("Invalid position format");
			}
		}

		m_document->InsertParagraph(text, position);
	}

	void ReplateText(std::istream& input)
	{
		size_t position;

		if (!(input >> position))
		{
			throw std::runtime_error("Position is not specified");
		}
		std::string text = ReadText(input);

		m_document->ReplaceText(text, position);
	}

	static std::string ReadText(std::istream& input)
	{
		std::string begin, end;
		if (input >> begin)
		{
			std::getline(input, end);
		}
		std::string text = begin + end;
		if (text.empty())
		{
			throw std::runtime_error("Text is not specified");
		}
		return text;
	}

	std::unique_ptr<IDocument> m_document;
	Menu m_menu;
	std::ostream& m_output;
};