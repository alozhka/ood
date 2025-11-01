#pragma once
#include "../DocumentItem.h"
#include "ICommand.h"

#include <memory>
#include <utility>

class ResizeImageCommand final : public ICommand
{
public:
	ResizeImageCommand(
		std::shared_ptr<IImage> image,
		int width,
		int height,
		size_t position)
		: m_image(std::move(image))
		, m_position(position)
		, m_width(width)
		, m_height(height)
	{
	}

	void Execute() override
	{
		m_oldWidth = m_image->GetWidth();
		m_oldHeight = m_image->GetHeight();
		m_image->Resize(m_width, m_height);
	}

	void Unexecute() override
	{
		m_image->Resize(m_oldWidth, m_oldHeight);
	}

	bool TryMerge(const ICommand* other) override
	{
		auto otherResize = dynamic_cast<const ResizeImageCommand*>(other);
		if (otherResize && otherResize->m_position == m_position)
		{
			m_width = otherResize->m_width;
			m_height = otherResize->m_height;
			return true;
		}
		return false;
	}

private:
	std::shared_ptr<IImage> m_image;
	size_t m_position;
	int m_width, m_height;
	int m_oldWidth = 0, m_oldHeight = 0;
};