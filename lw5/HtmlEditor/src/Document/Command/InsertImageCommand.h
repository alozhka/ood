#pragma once
#include "../DocumentItem.h"
#include "../Image.h"
#include "ICommand.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class InsertImageCommand final : public ICommand
{
public:
	InsertImageCommand(
		std::vector<std::shared_ptr<DocumentItem>>& items,
		size_t& imageCounter,
		const std::string& path,
		int width,
		int height,
		std::optional<size_t> position)
		: m_items(items)
		, m_imageCounter(imageCounter)
		, m_sourcePath(path)
		, m_width(width)
		, m_height(height)
		, m_position(position)
	{
	}

	void Execute() override
	{
		if (m_isMarkedForDeletion)
		{
			auto image = std::make_shared<Image>(m_imagePath, m_width, m_height);
			auto item = std::make_shared<DocumentItem>(image);
			m_items.insert(m_items.begin() + m_actualPosition, item);
			m_isMarkedForDeletion = false;
			++m_imageCounter;
			return;
		}

		if (!std::filesystem::exists(m_sourcePath))
		{
			throw std::runtime_error("Image file not found");
		}

		std::filesystem::create_directories("images");

		std::string extension = std::filesystem::path(m_sourcePath).extension().string();
		std::string newImageName = "image_" + std::to_string(++m_imageCounter) + extension;
		m_imagePath = "images/" + newImageName;

		std::filesystem::copy_file(m_sourcePath, m_imagePath, std::filesystem::copy_options::overwrite_existing);

		auto image = std::make_shared<Image>(m_imagePath, m_width, m_height);
		auto item = std::make_shared<DocumentItem>(image);

		size_t insertPos = m_position.value_or(m_items.size());
		m_items.insert(m_items.begin() + insertPos, item);

		m_actualPosition = insertPos;
	}

	void Unexecute() override
	{
		m_items.erase(m_items.begin() + m_actualPosition);
		// Помечаем файл для удаления вместо немедленного удаления
		m_isMarkedForDeletion = true;
		--m_imageCounter;
	}

	bool TryMerge(const ICommand* other) override
	{
		return false;
	}

	~InsertImageCommand() override
	{
		if (m_isMarkedForDeletion && std::filesystem::exists(m_imagePath))
		{
			std::filesystem::remove(m_imagePath);
		}
	}

private:
	std::vector<std::shared_ptr<DocumentItem>>& m_items;
	size_t& m_imageCounter;
	std::string m_sourcePath;
	std::string m_imagePath;
	int m_width;
	int m_height;
	std::optional<size_t> m_position;
	size_t m_actualPosition = 0;
	bool m_isMarkedForDeletion = false;
};