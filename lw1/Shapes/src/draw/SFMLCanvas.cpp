#include "SFMLCanvas.h"

gfx::SFMLCanvas::SFMLCanvas(sf::RenderWindow& window)
	: m_window(window)
	, m_currentColor(sf::Color::Black)
	, m_currentX(0)
	, m_currentY(0)
{
	// Кроссплатформенная загрузка шрифта
#ifdef _WIN32
	bool fontLoaded = m_font.openFromFile(R"(C:\Windows\Fonts\arial.ttf)");
#elif defined(__APPLE__)
	bool fontLoaded = m_font.openFromFile("/System/Library/Fonts/Geneva.ttf");
#else
	bool fontLoaded = m_font.openFromFile("/usr/share/fonts/TTF/arial.ttf");
#endif

	if (!fontLoaded)
	{
		throw std::runtime_error("Failed to load font");
	}
}

void gfx::SFMLCanvas::SetColor(Color c)
{
	m_currentColor = sf::Color(c.WithOpacity().m_color);
}

void gfx::SFMLCanvas::MoveTo(double x, double y)
{
	m_currentX = x;
	m_currentY = y;
}

void gfx::SFMLCanvas::LineTo(double x, double y)
{
	sf::Vertex line[2] = {
		sf::Vertex(sf::Vector2f(static_cast<float>(m_currentX), static_cast<float>(m_currentY)), m_currentColor),
		sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)), m_currentColor),
	};

	m_window.draw(line, 2, sf::PrimitiveType::Lines);

	m_currentX = x;
	m_currentY = y;
}

void gfx::SFMLCanvas::DrawEclipse(double cx, double cy, double rx, double ry)
{
	sf::CircleShape ellipse(static_cast<float>(rx));
	ellipse.setScale(sf::Vector2f(1.0f, static_cast<float>(ry / rx)));
	ellipse.setPosition(sf::Vector2f(static_cast<float>(cx - rx), static_cast<float>(cy - ry)));
	ellipse.setOutlineColor(m_currentColor);
	ellipse.setOutlineThickness(1);
	ellipse.setFillColor(m_currentColor);
	m_window.draw(ellipse);
}

void gfx::SFMLCanvas::DrawText(double left, double top, double fontSize, const std::string& text)
{
	sf::Text sfText(m_font, text, static_cast<unsigned int>(fontSize));
	sfText.setFillColor(m_currentColor);
	sfText.setPosition(sf::Vector2f(static_cast<float>(left), static_cast<float>(top)));
	m_window.draw(sfText);
}

void gfx::SFMLCanvas::Display()
{
	m_window.display();
}