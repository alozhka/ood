#pragma once
#include "ICommand.h"

class RenameTextCommand : public ICommand
{
public:
	RenameTextCommand(std::vector<std::shared_ptr<DocumentItem>>& m_items, const std::string& m_new_text, size_t m_position)
		: m_items(m_items)
		, m_newText(m_new_text)
		, m_position(m_position)
	{
	}
	void Execute() override
	{
		if (m_position >= m_items.size())
		{
			throw std::runtime_error("Position out of range");
		}

		std::shared_ptr<DocumentItem> item = m_items[m_position];
		item->SetParagraph();
	}

	void Unexecute() override
	{
		// TODO: implement
	}

private:
	std::vector<std::shared_ptr<DocumentItem>> m_items;
	std::string m_newText;
	size_t m_position;
};