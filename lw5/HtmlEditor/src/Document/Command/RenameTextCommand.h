#pragma once
#include "../DocumentItem.h"
#include "ICommand.h"

#include <memory>
#include <utility>

class RenameTextCommand final : public ICommand
{
public:
	RenameTextCommand(
		std::shared_ptr<IParagraph> paragraph,
		const std::string& newText,
		size_t position)
		: m_paragraph(std::move(paragraph))
		, m_text(newText)
		, m_position(position)
	{
	}

	void Execute() override
	{
		m_oldText = m_paragraph->GetText();
		m_paragraph->SetText(m_text);
	}

	void Unexecute() override
	{
		m_paragraph->SetText(m_oldText);
	}

	bool TryMerge(const ICommand* other) override
	{
		auto otherRename = dynamic_cast<const RenameTextCommand*>(other);
		if (otherRename && otherRename->m_position == m_position)
		{
			m_text = otherRename->m_text;
			return true;
		}
		return false;
	}

private:
	std::shared_ptr<IParagraph> m_paragraph;
	std::string m_oldText, m_text;
	size_t m_position;
};