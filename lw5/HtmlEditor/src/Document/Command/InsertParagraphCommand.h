#pragma once
#include "../DocumentItem.h"
#include "../Paragraph.h"
#include "ICommand.h"

#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

class InsertParagraphCommand : public ICommand
{
public:
	InsertParagraphCommand(
		std::vector<std::shared_ptr<DocumentItem>>& items,
		const std::string& text,
		std::optional<size_t> position)
		: m_items(items)
		, m_text(text)
		, m_position(position)
	{
	}

	void Execute() override
	{
		auto paragraph = std::make_shared<Paragraph>(m_text);
		m_item = std::make_shared<DocumentItem>(paragraph);

		size_t insertPos = m_position.value_or(m_items.size());

		if (insertPos > m_items.size())
		{
			throw std::out_of_range("Invalid position for insertion");
		}

		m_actualPosition = insertPos;
		m_items.insert(m_items.begin() + insertPos, m_item);
	}

	void Unexecute() override
	{
		if (m_actualPosition >= m_items.size())
		{
			throw std::logic_error("Cannot unexecute: invalid position");
		}

		m_items.erase(m_items.begin() + m_actualPosition);
	}

private:
	std::vector<std::shared_ptr<DocumentItem>>& m_items;
	std::string m_text;
	std::optional<size_t> m_position;
	std::shared_ptr<DocumentItem> m_item;
	size_t m_actualPosition = 0;
};