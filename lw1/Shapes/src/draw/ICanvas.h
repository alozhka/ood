#pragma once

#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

namespace Regexes
{
static const std::regex COLOR_REGEX("^[0-9a-fA-F]{6}$");
}

namespace gfx
{
struct Color
{
	explicit Color(uint32_t color)
		: m_color(color)
	{
	}

	static Color FromHex(const std::string& hex)
	{
		std::string hexStr = hex;
		if (hexStr[0] == '#')
		{
			hexStr = hexStr.substr(1);
		}

		if (!std::regex_match(hexStr, Regexes::COLOR_REGEX))
		{
			throw std::invalid_argument("Invalid color format.");
		}

		uint32_t value = std::stoul(hexStr, nullptr, 16);
		return Color(value);
	}

	bool operator==(const Color& other) const
	{
		return m_color == other.m_color;
	}

	std::string ToString() const
	{
		std::ostringstream oss;
		oss << "#" << std::hex << std::setw(6) << std::setfill('0') << m_color;

		return oss.str();
	}

	uint32_t m_color;
};

class ICanvas
{
public:
	virtual void SetColor(Color c) = 0;
	virtual void MoveTo(double x, double y) = 0;
	virtual void LineTo(double x, double y) = 0;
	virtual void DrawEclipse(double cx, double cy, double rx, double ry) = 0;
	virtual void DrawText(double left, double top, double fontSize, const std::string& text) = 0;

	virtual ~ICanvas() = default;
};
} // namespace gfx