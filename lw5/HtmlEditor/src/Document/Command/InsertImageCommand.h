#pragma once
#include "../IDocument.h"
#include "ICommand.h"

#include <memory>
#include <optional>
#include <string>

class InsertImageCommand final : public ICommand
{
public:
	InsertImageCommand(
		std::shared_ptr<IDocument>& document,
		const std::string& path,
		int width,
		int height,
		std::optional<size_t> position)
		: m_document(document)
		, m_path(path)
		, m_width(width)
		, m_height(height)
		, m_position(position)
	{
	}

	void Execute() override
	{
		m_actualPosition = m_position.value_or(m_document->GetItemsCount());
		m_document->InsertImage(m_path, m_width, m_height, m_position);
	}

	void Unexecute() override
	{
		m_document->DeleteItem(m_actualPosition);
	}

private:
	std::shared_ptr<IDocument> m_document;
	std::string m_path;
	int m_width;
	int m_height;
	std::optional<size_t> m_position;
	size_t m_actualPosition = 0;
};