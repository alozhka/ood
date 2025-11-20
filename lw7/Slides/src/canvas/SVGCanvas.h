#pragma once
#include "ICanvas.h"

#include <format>
#include <iomanip>
#include <iosfwd>
#include <sstream>
#include <vector>

class SVGCanvas : public ICanvas
{
public:
	explicit SVGCanvas(unsigned width = 800, unsigned height = 600)
		: m_width(width)
		, m_height(height)
	{
		m_stream.str("");
		PrintSvgHeader();
	}

	void SetLineColor(RGBAColor color) override
	{
		m_lineColor = color;
	}

	void SetStrokeWidth(unsigned strokeWidth) override
	{
		m_strokeWidth = strokeWidth;
	}

	void BeginFill(RGBAColor color) override
	{
		m_fillColor = color;
		m_isFilling = true;
	}

	void EndFill() override
	{
		if (m_pathPoints.empty())
		{
			return;
		}

		m_stream << R"(<polygon points=")";
		for (size_t i = 0; i < m_pathPoints.size(); ++i)
		{
			if (i > 0)
			{
				m_stream << " ";
			}
			m_stream << std::format("{},{}", m_pathPoints[i].x, m_pathPoints[i].y);
		}
		m_stream << std::format(R"(" fill="{}" stroke="{}" stroke-width="{}" />)",
			ColorToHex(m_fillColor),
			ColorToHex(m_lineColor),
			m_strokeWidth);
		m_stream << "\n";

		m_pathPoints.clear();
		m_isFilling = false;
	}

	void MoveTo(double x, double y) override
	{
		m_pathPoints.clear();
		m_pathPoints.push_back({ x, y });
	}

	void LineTo(double x, double y) override
	{
		m_pathPoints.push_back({ x, y });
	}

	void DrawEllipse(double left, double top, double width, double height) override
	{
		double cx = left + width / 2.0;
		double cy = top + height / 2.0;
		double rx = width / 2.0;
		double ry = height / 2.0;

		m_stream << std::format(
			R"(<ellipse cx="{}" cy="{}" rx="{}" ry="{}" fill="{}" stroke="{}" stroke-width="{}" />)",
			cx, cy, rx, ry,
			m_isFilling ? ColorToHex(m_fillColor) : "none",
			ColorToHex(m_lineColor),
			m_strokeWidth);
		m_stream << "\n";
	}

	void Export(std::ostream& output) override
	{
		m_stream << "</svg>\n";
		output << m_stream.str();
	}

private:
	void PrintSvgHeader()
	{
		m_stream << std::format(
			R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="{}" height="{}">)",
			m_width,
			m_height);
		m_stream << "\n";
	}

	static std::string ColorToHex(RGBAColor color)
	{
		// RGBA format: 0xRRGGBBAA
		unsigned char r = (color >> 24) & 0xFF;
		unsigned char g = (color >> 16) & 0xFF;
		unsigned char b = (color >> 8) & 0xFF;
		unsigned char a = color & 0xFF;

		std::ostringstream oss;
		oss << "#" << std::hex << std::setfill('0')
			<< std::setw(2) << static_cast<int>(r)
			<< std::setw(2) << static_cast<int>(g)
			<< std::setw(2) << static_cast<int>(b);

		// Добавляем прозрачность, если она не полная
		if (a != 0xFF)
		{
			oss << std::setw(2) << static_cast<int>(a);
		}

		return oss.str();
	}

	unsigned m_strokeWidth = 1;
	unsigned m_width;
	unsigned m_height;
	RGBAColor m_lineColor = 0x000000FF;
	RGBAColor m_fillColor = 0xFFFFFFFF;
	bool m_isFilling = false;

	struct Point
	{
		double x, y;
	};
	std::vector<Point> m_pathPoints;

	std::ostringstream m_stream;
};