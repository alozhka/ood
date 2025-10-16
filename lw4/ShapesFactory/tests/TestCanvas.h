#pragma once
#include "../src/canvas/ICanvas.h"

#include <vector>

class TestCanvas final : public ICanvas
{
public:
	void SetColor(Color color) override
	{
		m_results.emplace_back("set color");
	}
	void DrawLine(Point from, Point to) override
	{
		m_results.emplace_back("draw line");
	}
	void DrawEllipse(Point center, double rx, double ry) override
	{
		m_results.emplace_back("draw ellipse");
	}

	std::vector<std::string>& GetResults() { return m_results; }

private:
	std::vector<std::string> m_results{};
};