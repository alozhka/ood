#pragma once
#include "ICanvas.h"
#include <SFML/Graphics.hpp>

namespace gfx
{
class SFMLCanvas : public ICanvas
{
public:
	explicit SFMLCanvas(sf::RenderWindow& window);

	void SetColor(Color c) override;
	void MoveTo(double x, double y) override;
	void LineTo(double x, double y) override;
	void DrawEclipse(double cx, double cy, double rx, double ry) override;
	void DrawText(double left, double top, double fontSize, const std::string& text) override;

private:
	sf::RenderWindow& m_window;
	sf::Color m_currentColor;
	double m_currentX, m_currentY;
	sf::Font m_font;
};

} // namespace gfx