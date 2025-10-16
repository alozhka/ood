#pragma once
#include "IInputDataStream.h"
#include <fstream>
#include <stdexcept>
#include <string>

class FileInputStream : public IInputDataStream
{
public:
	explicit FileInputStream(const std::string& fileName)
		: m_file(fileName, std::ios::binary)
	{
		if (!m_file.is_open())
		{
			throw std::ios_base::failure("Failed to open file: " + fileName);
		}
	}

	bool IsEOF() const override
	{
		EnsureIsOpened();
		return m_file.eof() || m_file.peek() == EOF;
	}

	uint8_t ReadByte() override
	{
		EnsureIsOpened();

		char byte;
		if (!m_file.read(&byte, 1))
		{
			throw std::ios_base::failure("Failed to read byte from file");
		}

		return static_cast<uint8_t>(byte);
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		EnsureIsOpened();

		m_file.read(static_cast<char*>(dstBuffer), size);

		if (m_file.bad())
		{
			throw std::ios_base::failure("Failed to read block from file");
		}

		return m_file.gcount();
	}

	void Close() override
	{
		if (m_isOpened)
		{
			m_file.close();
			m_isOpened = false;
		}
	}

private:
	void EnsureIsOpened() const
	{
		if (!m_isOpened)
		{
			throw std::logic_error("Stream is closed");
		}
	}

	mutable std::ifstream m_file;
	bool m_isOpened = true;
};