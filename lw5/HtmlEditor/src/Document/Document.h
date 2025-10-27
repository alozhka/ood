#pragma once
#include "Command/InsertParagraphCommand.h"
#include "Command/RenameTextCommand.h"
#include "DocumentItem.h"
#include "History.h"
#include "IDocument.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Document : public IDocument
{
public:
	void InsertParagraph(const std::string& text, std::optional<size_t> position) override
	{
		auto paragraph = std::make_shared<Paragraph>(text);
		auto item = std::make_shared<DocumentItem>(paragraph);

		size_t insertPos = position.value_or(m_items.size());

		if (insertPos > m_items.size())
		{
			throw std::out_of_range("Invalid position for insertion");
		}

		m_items.insert(m_items.begin() + insertPos, item);
	}

	void ReplaceText(const std::string& newText, size_t position) override
	{
		if (position >= m_items.size())
		{
			throw std::runtime_error("Position out of range");
		}

		std::shared_ptr<IParagraph> paragraph = m_items[position]->GetParagraph();
		paragraph->SetText(newText);
	}

	void InsertImage(const std::string& path, int width, int height, std::optional<size_t> position) override
	{
		// TODO: implement
	}

	void ResizeImage(int width, int height, size_t position) override
	{
		// TODO: implement
	}

	size_t GetItemsCount() const override
	{
		return m_items.size();
	}

	std::shared_ptr<DocumentItem> GetItem(size_t index) const override
	{
		if (index >= m_items.size())
		{
			throw std::out_of_range("Invalid item index");
		}
		return m_items[index];
	}

	void DeleteItem(size_t index) override
	{
		// TODO: implement
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
		return false;
	}

	void Undo() override
	{
		// TODO: implement
	}

	bool CanRedo() const override
	{
		return false;
	}

	void Redo() override
	{
		// TODO: implement
	}

	void Save(const std::string& path) override
	{
		// TODO: implement
	}

private:
	std::string m_title{};
	std::vector<std::shared_ptr<DocumentItem>> m_items;
	History m_history{};
};