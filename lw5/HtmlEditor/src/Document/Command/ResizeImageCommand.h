#pragma once
#include "../DocumentItem.h"
#include "ICommand.h"

#include <memory>
#include <vector>

class ResizeImageCommand final : public ICommand
{
public:
	ResizeImageCommand(
		std::vector<std::shared_ptr<DocumentItem>>& items,
		size_t position,
		int width,
		int height)
		: m_items(items)
		, m_position(position)
		, m_width(width)
		, m_height(height)
	{
		auto image = m_items[m_position]->GetImage();
		if (!image)
		{
			throw std::runtime_error("Item is not an image");
		}
		m_oldWidth = image->GetWidth();
		m_oldHeight = image->GetHeight();
	}

	void Execute() override
	{
		auto image = m_items[m_position]->GetImage();
		image->Resize(m_width, m_height);
	}

	void Unexecute() override
	{
		auto image = m_items[m_position]->GetImage();
		image->Resize(m_oldWidth, m_oldHeight);
	}

private:
	std::vector<std::shared_ptr<DocumentItem>>& m_items;
	size_t m_position;
	int m_width, m_height;
	int m_oldWidth, m_oldHeight;
};