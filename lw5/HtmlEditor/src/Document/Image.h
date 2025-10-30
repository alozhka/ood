#pragma once
#include "IImage.h"
#include <filesystem>
#include <memory>
#include <stdexcept>

class History;

class Image : public IImage
{
public:
	Image(const std::string& path, int width, int height)
		: m_path(path)
		, m_width(width)
		, m_height(height)
	{
		EnsureDimensionsAreValid(width, height);
		m_width = width;
		m_height = height;
	}

	std::string GetPath() const override
	{
		return m_path;
	}

	int GetWidth() const override
	{
		return m_width;
	}

	int GetHeight() const override
	{
		return m_height;
	}

	void Resize(int width, int height) override
	{
		EnsureDimensionsAreValid(width, height);
		m_width = width;
		m_height = height;
	}

private:
	static void EnsureDimensionsAreValid(int width, int height)
	{
		if (width < 1 || width > 10000 || height < 1 || height > 10000)
		{
			throw std::invalid_argument("Invalid image dimensions (must be 1-10000)");
		}
	}

	std::string m_path;
	int m_width;
	int m_height;
};