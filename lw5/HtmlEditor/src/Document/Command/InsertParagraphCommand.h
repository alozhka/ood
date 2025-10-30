#pragma once
#include "../IDocument.h"
#include "ICommand.h"

#include <memory>
#include <optional>
#include <vector>

class InsertParagraphCommand final : public ICommand
{
public:
	InsertParagraphCommand(
		std::shared_ptr<IDocument>& document,
		const std::string& text,
		std::optional<size_t> position)
		: m_document(document)
		, m_text(text)
		, m_position(position)
	{
	}

	void Execute() override
	{
		m_document->InsertParagraph(m_text, m_position);
	}

	void Unexecute() override
	{
		m_document->DeleteItem(m_position.value_or(m_document->GetItemsCount()));
	}

private:
	std::shared_ptr<IDocument> m_document;
	std::string m_text;
	std::optional<size_t> m_position;
};