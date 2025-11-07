#pragma once
#include "../graphics_lib.h"
#include "../modern_graphics_lib.h"

class ModernGraphicsAdapter final : public graphics_lib::ICanvas
{
public:
	explicit ModernGraphicsAdapter(modern_graphics_lib::ModernGraphicsRenderer& renderer)
		: m_renderer(renderer)
	{
	}

	void BeginDraw()
	{
		m_renderer.BeginDraw();
	}

	void EndDraw()
	{
		m_renderer.EndDraw();
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
		modern_graphics_lib::Point m_destination{ x, y };
		m_renderer.DrawLine(m_position, m_destination, m_color);
		m_position = m_destination;
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
	modern_graphics_lib::ModernGraphicsRenderer& m_renderer;
};