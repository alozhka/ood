#pragma once
#include "../IDocument.h"
#include "ICommand.h"
#include "../DocumentItem.h"

#include <memory>

class DeleteItemCommand final : public ICommand
{
public:
	DeleteItemCommand(std::shared_ptr<IDocument>& m_document, size_t m_position)
		: m_document(m_document)
		, m_position(m_position)
	{
	}

	void Execute() override
	{
		m_deletedItem = m_document->GetItem(m_position);
		m_document->DeleteItem(m_position);
	}

	void Unexecute() override
	{
		// TODO: implement
	}

private:
	std::shared_ptr<IDocument> m_document;
	size_t m_position;
	std::shared_ptr<DocumentItem> m_deletedItem;
};