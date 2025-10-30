#pragma once
#include <utility>

#include "../IDocument.h"

class ResizeImageCommand final : public ICommand
{
public:
	ResizeImageCommand(
		std::shared_ptr<IDocument> document,
		size_t m_position,
		int m_width,
		int m_height)
		: m_document(std::move(document))
		, m_position(m_position)
		, m_width(m_width)
		, m_height(m_height)
	{
		std::shared_ptr<DocumentItem> item = m_document->GetItem(m_position);
		std::shared_ptr<IImage> image = item->GetImage();
		if (!image)
		{
			throw std::runtime_error("Item is not an image");
		}
		m_oldWidth = image->GetWidth();
		m_oldHeight = image->GetHeight();
	}

	void Execute() override
	{
		m_document->ResizeImage(m_width, m_height, m_position);
	}

	void Unexecute() override
	{
		m_document->ResizeImage(m_oldWidth, m_oldHeight, m_position);
	}

private:
	std::shared_ptr<IDocument> m_document;
	size_t m_position;
	int m_width, m_height;
	int m_oldWidth, m_oldHeight;
};