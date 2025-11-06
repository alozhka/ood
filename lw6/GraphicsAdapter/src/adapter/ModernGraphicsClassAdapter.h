#pragma once
#include "../graphics_lib.h"
#include "../modern_graphics_lib.h"

class ModernGraphicsClassAdapter final : public graphics_lib::ICanvas
	, private modern_graphics_lib::ModernGraphicsRenderer
{
public:
	explicit ModernGraphicsClassAdapter(std::ostream& strm)
		: ModernGraphicsRenderer(strm)
	{
		BeginDraw();
	}

	void SetColor(uint32_t rgbColor) override
	{
		m_color = OldToModernColor(rgbColor);
	}

	void MoveTo(int x, int y) override
	{
		m_position.x = x;
		m_position.y = y;
	}
	void LineTo(int x, int y) override
	{
		const modern_graphics_lib::Point end{ x, y };
		DrawLine(m_position, end, m_color);
		m_position = end;
	}

private:
	static modern_graphics_lib::RGBAColor OldToModernColor(uint32_t color)
	{
		constexpr float colorScale = 1.0f / 255.0f;

		const auto red = static_cast<float>((color >> 16) & 0xFF) * colorScale;
		const auto green = static_cast<float>((color >> 8) & 0xFF) * colorScale;
		const auto blue = static_cast<float>(color & 0xFF) * colorScale;
		const auto alpha = ((color >> 24) & 0xFF) == 0 ? 1.0f : static_cast<float>((color >> 24) & 0xFF) * colorScale;

		return { red, green, blue, alpha };
	}

	modern_graphics_lib::RGBAColor m_color{ 0, 0, 0, 0 };
	modern_graphics_lib::Point m_position{ 0, 0 };
};