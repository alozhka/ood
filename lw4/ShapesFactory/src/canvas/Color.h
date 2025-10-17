#pragma once
#include <stdexcept>
#include <string>

enum class Color
{
	Red,
	Green,
	Blue,
	Pink,
	Yellow,
	Black,
};

static std::string ColorToHexString(Color color)
{
	switch (color)
	{
	case Color::Black:
		return "#000000";
	case Color::Red:
		return "#ff0000";
	case Color::Green:
		return "#00ff00";
	case Color::Blue:
		return "#0000ff";
	case Color::Yellow:
		return "#ffff00";
	case Color::Pink:
		return "#ffc0cb";
	default:
		throw std::invalid_argument("Unknown color");
	}
}

static Color ParseColor(const std::string& colorStr)
{
	std::string lowerColor = colorStr;
	std::ranges::transform(lowerColor, lowerColor.begin(), tolower);

	if (lowerColor == "red")
		return Color::Red;
	if (lowerColor == "green")
		return Color::Green;
	if (lowerColor == "blue")
		return Color::Blue;
	if (lowerColor == "pink")
		return Color::Pink;
	if (lowerColor == "yellow")
		return Color::Yellow;
	if (lowerColor == "black")
		return Color::Black;

	throw std::invalid_argument("Unknown color: " + colorStr);
}
