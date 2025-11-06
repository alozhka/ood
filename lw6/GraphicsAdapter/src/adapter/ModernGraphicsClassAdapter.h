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

	~ModernGraphicsClassAdapter() override
	{
		EndDraw();
	}

	void MoveTo(int x, int y) override
	{
		m_position.x = x;
		m_position.y = y;
	}
	void LineTo(int x, int y) override
	{
		const modern_graphics_lib::Point end{ x, y };
		DrawLine(m_position, end);
		m_position = end;
	}

private:
	modern_graphics_lib::Point m_position{ 0, 0 };
};