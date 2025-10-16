#pragma once
#include "../../output/decorators/OutputStreamDecorator.h"
#include "InputStreamDecorator.h"

class DecompressInputStream : public InputStreamDecorator
{
public:
	explicit DecompressInputStream(std::unique_ptr<IInputDataStream>&& stream)
		: InputStreamDecorator(std::move(stream))
	{
	}

	uint8_t ReadByte() override
	{
		if (m_byteCount > 0)
		{
			--m_byteCount;
			return m_currentByte;
		}

		uint8_t count = m_stream->ReadByte();
		m_currentByte = m_stream->ReadByte();
		m_byteCount = count - 1;

		return m_currentByte;
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		uint8_t* buffer = static_cast<uint8_t*>(dstBuffer);
		std::streamsize bytesRead = 0;

		for (std::streamsize i = 0; i < size; ++i)
		{
			if (IsEOF())
			{
				break;
			}
			buffer[i] = ReadByte();
			++bytesRead;
		}

		return bytesRead;
	}

	bool IsEOF() const override
	{
		return m_byteCount == 0 && m_stream->IsEOF();
	}

	void Close() override
	{
		m_stream->Close();
	}

private:
	size_t m_byteCount = 0;
	uint8_t m_currentByte = 0;
};