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

	void MoveTo(int x, int y) override
	{
		m_position.x = x;
		m_position.y = y;
	}

	void LineTo(int x, int y) override
	{
		modern_graphics_lib::Point m_destination{ x, y };
		m_renderer.DrawLine(m_position, m_destination);
		m_position = m_destination;
	}

private:
	modern_graphics_lib::Point m_position{ 0, 0 };
	modern_graphics_lib::ModernGraphicsRenderer& m_renderer;
};