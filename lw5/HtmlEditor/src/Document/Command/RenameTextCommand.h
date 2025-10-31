#pragma once
#include "../DocumentItem.h"
#include "ICommand.h"

#include <memory>
#include <vector>

class RenameTextCommand final : public ICommand
{
public:
	RenameTextCommand(
		std::vector<std::shared_ptr<DocumentItem>>& items,
		const std::string& newText,
		size_t position)
		: m_items(items)
		, m_newText(newText)
		, m_position(position)
	{
	}

	void Execute() override
	{
		auto paragraph = m_items[m_position]->GetParagraph();
		if (!paragraph)
		{
			throw std::runtime_error("Item is not a paragraph");
		}
		m_oldText = paragraph->GetText();
		paragraph->SetText(m_newText);
	}

	void Unexecute() override
	{
		auto paragraph = m_items[m_position]->GetParagraph();
		paragraph->SetText(m_oldText);
	}

	bool TryMerge(const ICommand* other) override
	{
		auto otherRename = dynamic_cast<const RenameTextCommand*>(other);
		if (otherRename && otherRename->m_position == m_position)
		{
			// TODO: покрыть тестами
			m_newText = otherRename->m_newText;
			return true;
		}
		return false;
	}

private:
	std::vector<std::shared_ptr<DocumentItem>>& m_items;
	std::string m_oldText{};
	std::string m_newText;
	size_t m_position;
};