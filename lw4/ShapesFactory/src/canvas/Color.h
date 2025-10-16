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
