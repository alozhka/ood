#pragma once

#include "../src/draw/ICanvas.h"
#include <string>

namespace gfx
{
class MockCanvas : public ICanvas
{
public:
	void SetColor(Color c) override
	{
		++m_counter;
	}

	void MoveTo(double x, double y) override
	{
		++m_counter;
	}

	void LineTo(double x, double y) override
	{
		++m_counter;
	}

	void DrawEclipse(double cx, double cy, double rx, double ry) override
	{
		++m_counter;
	}

	void DrawText(double left, double top, double fontSize, const std::string& text) override
	{
		++m_counter;
	}

	void Display() override
	{
		++m_counter;
	}

	size_t GetCallCount() const
	{
		return m_counter;
	}

private:
	size_t m_counter = 0;
};

} // namespace gfx