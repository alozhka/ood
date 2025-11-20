#pragma once
#include <cstdint>

using RGBAColor = uint32_t;

class Style
{
public:
	bool IsEnabled() const
	{
		return m_isEnabled;
	}

	void Enable(bool enable)
	{
		m_isEnabled = enable;
	}

	RGBAColor GetColor() const
	{
		return m_color;
	}

	void SetColor(RGBAColor color)
	{
		m_color = color;
	}

private:
	bool m_isEnabled{ true };
	RGBAColor m_color{ 0 };
};
