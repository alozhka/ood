#pragma once
#include "../DocumentItem.h"
#include "../Image.h"
#include "../Image/ImageService.h"
#include "ICommand.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class InsertImageCommand final : public ICommand
{
public:
	InsertImageCommand(
		std::vector<std::shared_ptr<DocumentItem>>& items,
		std::shared_ptr<DocumentItem> imageItem,
		const std::string& sourcePath,
		std::optional<size_t> position)
		: m_items(items)
		, m_imageItem(std::move(imageItem))
		, m_sourcePath(sourcePath)
		, m_position(position.value_or(items.size()))
	{
	}

	void Execute() override
	{
		if (m_isDeleted)
		{
			m_items.insert(m_items.begin() + m_position, m_imageItem);
			m_isDeleted = false;
			return;
		}

		std::filesystem::copy_file(
			m_sourcePath,
			m_imageItem->GetImage()->GetPath(),
			std::filesystem::copy_options::overwrite_existing);

		m_items.insert(m_items.begin() + m_position, m_imageItem);
	}

	void Unexecute() override
	{
		m_imageItem = m_items[m_position];
		m_items.erase(m_items.begin() + m_position);
		m_isDeleted = true;
	}

	bool TryMerge(const ICommand* other) override
	{
		return false;
	}

	~InsertImageCommand() override
	{
		if (m_isDeleted)
		{
			ImageService::RemoveImage(m_imageItem->GetImage()->GetPath());
		}
	}

private:
	std::vector<std::shared_ptr<DocumentItem>>& m_items;
	std::shared_ptr<DocumentItem> m_imageItem;
	std::string m_sourcePath;
	size_t m_position;
	bool m_isDeleted = false;
};