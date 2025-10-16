#pragma once
#include "../../decorators/OutputStreamDecorator.h"

class CompressionOutputStream : public OutputStreamDecorator
{
public:
	explicit CompressionOutputStream(std::unique_ptr<IOutputDataStream>&& stream)
		: OutputStreamDecorator(std::move(stream))
	{
	}

	void WriteByte(uint8_t data) override
	{
		if (m_currentCount == 0)
		{
			m_currentByte = data;
		}
		if (m_currentByte == data && m_currentCount != std::numeric_limits<uint8_t>::max())
		{
			++m_currentCount;
		}
		else
		{
			WriteCompressByte();
			m_currentCount = 1;
			m_currentByte = data;
		}
	}

	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		auto data = static_cast<const uint8_t*>(srcData);

		for (std::streamsize i = 0; i < size; ++i)
		{
			WriteByte(data[i]);
		}
	}

	void Close() override
	{
		WriteCompressByte();
		m_stream->Close();
	}

private:
	void WriteCompressByte()
	{
		if (m_currentCount > 2)
		{
			m_stream->WriteByte(m_currentCount);
			m_stream->WriteByte(m_currentByte);
		}
		if (m_currentCount == 2)
		{
			m_stream->WriteByte(m_currentByte);
			m_stream->WriteByte(m_currentByte);
		}
		if (m_currentCount == 1)
		{
			m_stream->WriteByte(m_currentByte);
		}

		m_currentCount = 0;
	}

	size_t m_currentCount = 0;
	uint8_t m_currentByte = 0;
};