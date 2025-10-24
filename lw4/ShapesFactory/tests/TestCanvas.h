#pragma once
#include "../src/canvas/ICanvas.h"

#include <vector>

class TestCanvas final : public ICanvas
{
public:
	void SetColor(Color color) override
	{
		std::vector<std::string> command{ "set color", ColorToHexString(color) };
		m_results.emplace_back(command);
	}

	void DrawLine(double fromX, double fromY, double toX, double toY) override
	{
		std::vector<std::string> command{ "draw line", std::to_string(fromX), std::to_string(fromY), std::to_string(toX), std::to_string(toY) };
		m_results.emplace_back(command);
	}

	void DrawEllipse(double cx, double cy, double rx, double ry) override
	{
		std::vector<std::string> command{ "draw ellipse", std::to_string(cx), std::to_string(cy), std::to_string(rx), std::to_string(ry) };
		m_results.emplace_back(command);
	}

	std::vector<std::vector<std::string>>& GetResults() { return m_results; }

private:
	std::vector<std::vector<std::string>> m_results{};
};