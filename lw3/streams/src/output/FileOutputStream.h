#pragma once
#include "IOutputDataStream.h"
#include <fstream>
#include <stdexcept>
#include <string>

class FileOutputStream : public IOutputDataStream
{
public:
	explicit FileOutputStream(const std::string& fileName)
		: m_file(fileName, std::ios::binary)
	{
		if (!m_file.is_open())
		{
			throw std::ios_base::failure("Failed to open file: " + fileName);
		}
	}

	void WriteByte(uint8_t data) override
	{
		EnsureStreamIsOpened();

		char byte = static_cast<char>(data);
		if (!m_file.write(&byte, 1))
		{
			throw std::ios_base::failure("Failed to write byte to file");
		}
	}

	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		EnsureStreamIsOpened();

		if (!m_file.write(static_cast<const char*>(srcData), size))
		{
			throw std::ios_base::failure("Failed to write block to file");
		}
	}

	void Close() override
	{
		if (!m_isClosed)
		{
			m_file.close();
			m_isClosed = true;
		}
	}

private:
	void EnsureStreamIsOpened() const
	{
		if (m_isClosed)
		{
			throw std::logic_error("Stream is closed");
		}
	}

	std::ofstream m_file;
	bool m_isClosed = false;
};