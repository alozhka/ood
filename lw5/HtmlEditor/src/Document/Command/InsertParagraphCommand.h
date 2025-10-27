#pragma once
#include "../DocumentItem.h"
#include "../IDocument.h"
#include "../Paragraph.h"
#include "ICommand.h"

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

class InsertParagraphCommand : public ICommand
{
public:
	InsertParagraphCommand(
		std::shared_ptr<IDocument> document,
		const std::string& text,
		std::optional<size_t> position)
		: m_document(std::move(document))
		, m_text(text)
		, m_position(position)
	{
	}

	void Execute() override
	{
		m_actualPosition = m_position.value_or(m_document->GetItemsCount());
		m_document->InsertParagraph(m_text, m_position);
	}

	void Unexecute() override
	{
		m_document->DeleteItem(m_actualPosition);
	}

private:
	std::shared_ptr<IDocument> m_document;
	std::string m_text;
	std::optional<size_t> m_position;
	size_t m_actualPosition = 0;
};