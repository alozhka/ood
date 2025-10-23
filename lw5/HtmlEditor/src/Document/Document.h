#pragma once
#include "History.h"
#include "IDocument.h"

#include <string>

class Document : public IDocument
{
public:
	void InsertParagraph(const std::string& text, std::optional<size_t> position) override
	{
		// TODO: implement
	}

	void ReplaceText(const std::string& newText, size_t position) override
	{
		// TODO: implement
	}

	void InsertImage(const std::string& path, int width, int height, std::optional<size_t> position) override
	{
		// TODO: implement
	}

	void ResizeImage(int width, int height, size_t position) override
	{
		// TODO: implement
	}

	size_t GetItemsCount() const override
	{
		return 0;
	}

	void DeleteItem(size_t index) override
	{
		// TODO: implement
	}

	std::string GetTitle() const override
	{
		return m_title;
	}

	void SetTitle(const std::string& title) override
	{
		m_title = title;
	}

	bool CanUndo() const override
	{
		return false;
	}

	void Undo() override
	{
		// TODO: implement
	}

	bool CanRedo() const override
	{
		return false;
	}

	void Redo() override
	{
		// TODO: implement
	}

	void Save(const std::string& path) override
	{
		// TODO: implement
	}

private:
	std::string m_title{};
	History history{};
};