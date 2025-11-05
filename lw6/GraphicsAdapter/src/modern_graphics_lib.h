#pragma once
#include <ostream>

// Пространство имен современной графической библиотеки (недоступно для изменения)
namespace modern_graphics_lib
{

class Point
{
public:
	Point(int x, int y)
		: x(x)
		, y(y)
	{
	}
	int x;
	int y;
};

// Класс для современного рисования графики
class ModernGraphicsRenderer
{
public:
	explicit ModernGraphicsRenderer(std::ostream& strm)
		: m_out(strm)
	{
	}

	~ModernGraphicsRenderer()
	{
		if (m_drawing) // Завершаем рисование, если оно было начато
		{
			EndDraw();
		}
	}

	// Этот метод должен быть вызван в начале рисования
	void BeginDraw()
	{
		if (m_drawing)
		{
			throw std::logic_error("Drawing has already begun");
		}
		m_out << "<draw>" << std::endl;
		m_drawing = true;
	}

	// Выполняет рисование линии
	void DrawLine(const Point& start, const Point& end)
	{
		if (!m_drawing)
		{
			throw std::logic_error("DrawLine is allowed between BeginDraw()/EndDraw() only");
		}
		m_out << std::format(
			R"(  <line fromX="{}" fromY="{}" toX="{}" toY="{}"/>)",
			start.x,
			start.y,
			end.x,
			end.y)
			  << std::endl;
	}

	// Этот метод должен быть вызван в конце рисования
	void EndDraw()
	{
		if (!m_drawing)
		{
			throw std::logic_error("Drawing has not been started");
		}
		m_out << "</draw>" << std::endl;
		m_drawing = false;
	}

private:
	std::ostream& m_out;
	bool m_drawing = false;
};
} // namespace modern_graphics_lib