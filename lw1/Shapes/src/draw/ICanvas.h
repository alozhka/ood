#pragma once

#include <regex>
#include <string>

namespace Regexes
{
static const std::regex COLOR_REGEX("^[0-9a-fA-F]{6}$");
}

namespace gfx
{
struct Color
{
	Color(uint8_t red, uint8_t green, uint8_t blue)
		: R(red)
		, G(green)
		, B(blue)
	{
	}

	static Color FromHex(const std::string& hex)
	{
		std::string hexStr = hex;
		if (hexStr[0] == '#')
		{
			hexStr = hexStr.substr(1);
		}

		if (!std::regex_match(hex, Regexes::COLOR_REGEX))
		{
			throw std::invalid_argument("Invalid color format.");
		}

		uint32_t value = std::stoul(hexStr, nullptr, 16);
		return {
			static_cast<uint8_t>((value >> 16) & 0xFF),
			static_cast<uint8_t>((value >> 8) & 0xFF),
			static_cast<uint8_t>(value & 0xFF)
		};
	}

	uint8_t R, G, B;
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