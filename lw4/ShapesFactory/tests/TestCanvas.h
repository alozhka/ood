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

	void DrawLine(double fromX, double fromY, double toX, double toY) override
	{
		m_results.emplace_back("draw line");
	}

	void DrawEllipse(double cx, double cy, double rx, double ry) override
	{
		m_results.emplace_back("draw ellipse");
	}

	std::vector<std::string>& GetResults() { return m_results; }

private:
	std::vector<std::string> m_results{};
};