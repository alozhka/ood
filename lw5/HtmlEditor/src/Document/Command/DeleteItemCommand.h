#pragma once
#include "../DocumentItem.h"
#include "ICommand.h"

#include <memory>
#include <vector>

class DeleteItemCommand final : public ICommand
{
public:
	DeleteItemCommand(
		std::vector<std::shared_ptr<DocumentItem>>& items,
		size_t position)
		: m_items(items)
		, m_position(position)
	{
	}

	void Execute() override
	{
		m_deletedItem = m_items[m_position];
		m_items.erase(m_items.begin() + m_position);
	}

	void Unexecute() override
	{
		m_items.insert(m_items.begin() + m_position, m_deletedItem);
	}

private:
	std::vector<std::shared_ptr<DocumentItem>>& m_items;
	size_t m_position;
	std::shared_ptr<DocumentItem> m_deletedItem;
};