#pragma once
#include "Document/Document.h"
#include "Document/IDocument.h"
#include "Menu.h"

#include <filesystem>

class CommandController
{
public:
	explicit CommandController(std::istream& input, std::ostream& output)
		: m_menu{ input, output }
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
		m_menu.AddItem(
			"DeleteItem",
			"Usage: DeleteItem <position>. Deletes the item at specified position.",
			std::bind_front(&CommandController::DeleteItem, this));
		m_menu.AddItem(
			"InsertImage",
			"Usage: InsertImage <position>|end <width> <height> <path>. Inserts an image.",
			std::bind_front(&CommandController::InsertImage, this));
		m_menu.AddItem(
			"ResizeImage",
			"Usage: ResizeImage <position> <width> <height>. Resizes an image.",
			std::bind_front(&CommandController::ResizeImage, this));
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
			else if (auto image = item->GetImage())
			{
				m_output << "Image: " << image->GetWidth() << " " << image->GetHeight() << " " << image->GetPath() << std::endl;
			}
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

		std::optional<size_t> position = PositionStringToNumber(positionStr);

		m_document->InsertParagraph(text, position);
	}

	void ReplateText(std::istream& input)
	{
		size_t position;

		if (!(input >> position))
		{
			throw std::invalid_argument("Position is not specified");
		}

		if (position == 0)
		{
			throw std::invalid_argument("Position must be >= 1");
		}

		std::string text = ReadText(input);

		--position;
		m_document->ReplaceText(text, position);
	}

	void DeleteItem(std::istream& input)
	{
		size_t position;
		if (!(input >> position))
		{
			throw std::invalid_argument("Position is not spesifiedß");
		}

		--position;
		m_document->DeleteItem(position);
	}

	void InsertImage(std::istream& input)
	{
		std::string positionStr;
		int width, height;
		std::string path;

		if (!(input >> positionStr >> width >> height >> path))
		{
			throw std::runtime_error("Not all arguments are specified");
		}
		std::optional<size_t> position = PositionStringToNumber(positionStr);

		m_document->InsertImage(path, width, height, position);
	}

	void ResizeImage(std::istream& input)
	{
		size_t position;
		int width, height;
		if (!(input >> position >> width >> height))
		{
			throw std::runtime_error("Not all arguments are specified");
		}
		--position;

		m_document->ResizeImage(width, height, position);
	}

	static std::optional<size_t> PositionStringToNumber(const std::string& positionStr)
	{
		if (positionStr != "end")
		{
			try
			{
				size_t userPosition = std::stoull(positionStr);
				if (userPosition == 0)
				{
					throw std::out_of_range("Position must be >= 1");
				}
				return userPosition - 1;
			}
			catch (const std::invalid_argument&)
			{
				throw std::invalid_argument("Invalid position format");
			}
		}
		return std::nullopt;
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

	std::shared_ptr<IDocument> m_document = std::make_shared<Document>();
	Menu m_menu;
	std::ostream& m_output;
};