#pragma once
#include "../DocumentItem.h"
#include "../Paragraph.h"
#include "ICommand.h"

#include <memory>
#include <optional>
#include <vector>

class InsertParagraphCommand final : public ICommand
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
		auto item = std::make_shared<DocumentItem>(paragraph);

		size_t insertPos = m_position.value_or(m_items.size());
		m_items.insert(m_items.begin() + insertPos, item);

		// Сохраняем фактическую позицию вставки для отмены
		m_actualPosition = insertPos;
	}

	void Unexecute() override
	{
		m_items.erase(m_items.begin() + m_actualPosition);
	}

private:
	std::vector<std::shared_ptr<DocumentItem>>& m_items;
	std::string m_text;
	std::optional<size_t> m_position;
	size_t m_actualPosition = 0;
};