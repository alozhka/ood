#pragma once
#include "DocumentItem.h"
#include "IDocument.h"
#include "Image.h"
#include "Paragraph.h"
#include "History.h"
#include "Command/InsertParagraphCommand.h"
#include "Command/InsertImageCommand.h"
#include "Command/DeleteItemCommand.h"
#include "Command/RenameTextCommand.h"
#include "Command/ResizeImageCommand.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Document : public IDocument
{
public:
	void InsertParagraph(const std::string& text, std::optional<size_t> position) override
	{
		size_t insertPos = position.value_or(m_items.size());
		EnsurePositionValidForInsertion(insertPos);

		auto command = std::make_unique<InsertParagraphCommand>(m_items, text, position);
		m_history.AddAndExecute(std::move(command));
	}

	void ReplaceText(const std::string& newText, size_t position) override
	{
		EnsureIndexInRange(position);

		auto command = std::make_unique<RenameTextCommand>(m_items, newText, position);
		m_history.AddAndExecute(std::move(command));
	}

	void InsertImage(const std::string& path, int width, int height, std::optional<size_t> position) override
	{
		size_t insertPos = position.value_or(m_items.size());
		EnsurePositionValidForInsertion(insertPos);

		auto command = std::make_unique<InsertImageCommand>(m_items, m_imageCounter, path, width, height, position);
		m_history.AddAndExecute(std::move(command));
	}

	void ResizeImage(int width, int height, size_t position) override
	{
		EnsureIndexInRange(position);

		auto command = std::make_unique<ResizeImageCommand>(m_items, position, width, height);
		m_history.AddAndExecute(std::move(command));
	}

	size_t GetItemsCount() const override
	{
		return m_items.size();
	}

	std::shared_ptr<DocumentItem> GetItem(size_t index) const override
	{
		EnsureIndexInRange(index);
		return m_items[index];
	}

	void DeleteItem(size_t index) override
	{
		EnsureIndexInRange(index);

		auto command = std::make_unique<DeleteItemCommand>(m_items, index);
		m_history.AddAndExecute(std::move(command));
	}

	std::string GetTitle() const override
	{
		return m_title;
	}

	void SetTitle(const std::string& title) override
	{
		m_title = title;
	}

	bool CanUndo() const override
	{
		return m_history.CanUndo();
	}

	void Undo() override
	{
		m_history.Undo();
	}

	bool CanRedo() const override
	{
		return m_history.CanRedo();
	}

	void Redo() override
	{
		m_history.Redo();
	}

	void Save(const std::string& path) override
	{
		std::ofstream file(path);
		if (!file.is_open())
		{
			throw std::runtime_error("Cannot open file for writing");
		}

		// Записываем HTML структуру
		file << "<!DOCTYPE html>\n";
		file << "<html>\n";
		file << "<head>\n";
		file << "<title>" << HtmlEscape(m_title) << "</title>\n";
		file << "</head>\n";
		file << "<body>\n";

		// Записываем элементы документа
		for (const auto& item : m_items)
		{
			if (auto paragraph = item->GetParagraph())
			{
				file << "<p>" << HtmlEscape(paragraph->GetText()) << "</p>\n";
			}
			else if (auto image = item->GetImage())
			{
				file << "<img src=\"" << HtmlEscape(image->GetPath())
					 << "\" width=\"" << image->GetWidth()
					 << "\" height=\"" << image->GetHeight()
					 << "\" />\n";
			}
		}

		file << "</body>\n";
		file << "</html>\n";

		file.close();
	}

private:
	void EnsureIndexInRange(size_t index) const
	{
		if (index >= m_items.size())
		{
			throw std::out_of_range("Invalid item index");
		}
	}

	void EnsurePositionValidForInsertion(size_t position) const
	{
		if (position > m_items.size())
		{
			throw std::out_of_range("Invalid position for insertion");
		}
	}

	static std::string HtmlEscape(const std::string& text)
	{
		std::string result;
		result.reserve(text.size());

		for (char ch : text)
		{
			switch (ch)
			{
			case '<':
				result += "&lt;";
				break;
			case '>':
				result += "&gt;";
				break;
			case '&':
				result += "&amp;";
				break;
			case '"':
				result += "&quot;";
				break;
			case '\'':
				result += "&apos;";
				break;
			default:
				result += ch;
				break;
			}
		}

		return result;
	}

	std::string m_title{};
	std::vector<std::shared_ptr<DocumentItem>> m_items;
	size_t m_imageCounter = 0;
	History m_history;
};