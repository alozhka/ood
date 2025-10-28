#pragma once
#include <utility>

#include "ICommand.h"

class RenameTextCommand final : public ICommand
{
public:
	RenameTextCommand(std::shared_ptr<IDocument> document, const std::string& newText, size_t position)
		: m_document(std::move(document))
		, m_newText(newText)
		, m_position(position)
	{
	}
	void Execute() override
	{
		m_oldText = m_document->GetItem(m_position)->GetParagraph()->GetText();
		m_document->ReplaceText(m_newText, m_position);
	}

	void Unexecute() override
	{
		m_document->ReplaceText(m_oldText, m_position);
	}

private:
	std::shared_ptr<IDocument> m_document;
	std::string m_oldText{}, m_newText;
	size_t m_position;
};