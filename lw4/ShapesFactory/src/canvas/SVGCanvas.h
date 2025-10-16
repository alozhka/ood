#pragma once
#include "ICanvas.h"

#include <fstream>
#include <sstream>

class SVGCanvas : public ICanvas
{
public:
	explicit SVGCanvas()
		: m_currentColor(Color::Black)
	{
	}

	void SetColor(Color color) override
	{
		m_currentColor = color;
	}

	void DrawLine(Point from, Point to) override
	{
		std::string line = "<line x1=\"" + std::to_string(from.x) + "\" "
			+ "y1=\"" + std::to_string(from.y) + "\" "
			+ "x2=\"" + std::to_string(to.x) + "\" "
			+ "y2=\"" + std::to_string(to.y) + "\" "
			+ "style=\"stroke:" + ColorToHexString(m_currentColor) + ";stroke=width:4\"/>";
		m_output
			<< line << std::endl;
	}

	void DrawEllipse(Point center, double rx, double ry) override
	{
		std::string ellipse = "<ellipse cx=\"" + std::to_string(center.x) + "\" "
			+ "cy=\"" + std::to_string(center.y) + "\" "
			+ "rx=\"" + std::to_string(rx) + "\" "
			+ "ry=\"" + std::to_string(ry) + "\" "
			+ "style=\"stroke:" + ColorToHexString(m_currentColor) + ";stroke-width:4;fill:none\"/>";
		m_output << ellipse << std::endl;
	}

	void SaveToFile(const std::string& filename)
	{
		std::ofstream file(filename);

		if (!file.is_open())
		{
			throw std::invalid_argument("Can't open file " + filename);
		}

		file << R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1">)";
		file << m_output.str() << std::endl;
		file << R"(</svg>)";
	}

private:
	Color m_currentColor;
	std::ostringstream m_output;
};