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
			m_stream->ReadByte();
		}
	}
	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
	}
	bool IsEOF() const override;
	void Close() override;

private:
	size_t m_byteCount = 0;
	uint8_t m_currentByte = 0;
};