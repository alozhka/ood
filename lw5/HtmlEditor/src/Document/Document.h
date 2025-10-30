#pragma once
#include "DocumentItem.h"
#include "IDocument.h"
#include "Image.h"
#include "Paragraph.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Document : public IDocument
{
public:
	void InsertParagraph(const std::string& text, std::optional<size_t> position) override
	{
		auto paragraph = std::make_shared<Paragraph>(text);
		auto item = std::make_shared<DocumentItem>(paragraph);

		size_t insertPos = position.value_or(m_items.size());

		if (insertPos > m_items.size())
		{
			throw std::out_of_range("Invalid position for insertion");
		}

		m_items.insert(m_items.begin() + insertPos, item);
	}

	void ReplaceText(const std::string& newText, size_t position) override
	{
		if (position >= m_items.size())
		{
			throw std::runtime_error("Position out of range");
		}

		std::shared_ptr<IParagraph> paragraph = m_items[position]->GetParagraph();
		paragraph->SetText(newText);
	}

	void InsertImage(const std::string& path, int width, int height, std::optional<size_t> position) override
	{
		// Проверяем существование файла
		if (!std::filesystem::exists(path))
		{
			throw std::runtime_error("Image file not found");
		}

		size_t insertPos = position.value_or(m_items.size());

		if (insertPos > m_items.size())
		{
			throw std::out_of_range("Invalid position for insertion");
		}

		// Создаём каталог images, если его нет
		std::filesystem::create_directories("images");

		// Генерируем имя для изображения
		std::string extension = std::filesystem::path(path).extension().string();
		std::string newImageName = "image_" + std::to_string(++m_imageCounter) + extension;
		std::string newImagePath = "images/" + newImageName;

		// Копируем файл
		std::filesystem::copy_file(path, newImagePath, std::filesystem::copy_options::overwrite_existing);

		auto image = std::make_shared<Image>(newImagePath, width, height);
		auto item = std::make_shared<DocumentItem>(image);

		m_items.insert(m_items.begin() + insertPos, item);
	}

	void ResizeImage(int width, int height, size_t position) override
	{
		EnsureIndexInRange(position);
		std::shared_ptr<DocumentItem> item = GetItem(position);
		std::shared_ptr<IImage> image = item->GetImage();
		if (!image)
		{
			throw std::runtime_error("Item is not an image");
		}

		image->Resize(width, height);
	}

	size_t GetItemsCount() const override
	{
		return m_items.size();
	}

	std::shared_ptr<DocumentItem> GetItem(size_t index) const override
	{
		EnsureIndexInRange(index);
		return m_items[index];
	}

	void DeleteItem(size_t index) override
	{
		EnsureIndexInRange(index);
		m_items.erase(m_items.begin() + index);
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
	void EnsureIndexInRange(size_t index) const
	{
		if (index >= m_items.size())
		{
			throw std::out_of_range("Invalid item index");
		}
	}

	std::string m_title{};
	std::vector<std::shared_ptr<DocumentItem>> m_items;
	size_t m_imageCounter = 0;
};