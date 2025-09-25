#pragma once

#include "../src/draw/ICanvas.h"
#include <vector>
#include <string>

namespace gfx
{

struct DrawCall
{
    enum Type
    {
        SET_COLOR,
        MOVE_TO,
        LINE_TO,
        DRAW_ECLIPSE,
        DRAW_TEXT
    };

    Type type;
    Color color;
    double x, y;
    double rx, ry;
    double fontSize;
    std::string text;

    DrawCall(Type t, Color c) : type(t), color(c) {}
    DrawCall(Type t, double x, double y) : type(t), x(x), y(y) {}
    DrawCall(Type t, double cx, double cy, double rx, double ry) : type(t), x(cx), y(cy), rx(rx), ry(ry) {}
    DrawCall(Type t, double left, double top, double fontSize, const std::string& text)
        : type(t), x(left), y(top), fontSize(fontSize), text(text) {}
};

class MockCanvas : public ICanvas
{
public:
    void SetColor(Color c) override
    {
        m_calls.emplace_back(DrawCall::SET_COLOR, c);
    }

    void MoveTo(double x, double y) override
    {
        m_calls.emplace_back(DrawCall::MOVE_TO, x, y);
    }

    void LineTo(double x, double y) override
    {
        m_calls.emplace_back(DrawCall::LINE_TO, x, y);
    }

    void DrawEclipse(double cx, double cy, double rx, double ry) override
    {
        m_calls.emplace_back(DrawCall::DRAW_ECLIPSE, cx, cy, rx, ry);
    }

    void DrawText(double left, double top, double fontSize, const std::string& text) override
    {
        m_calls.emplace_back(DrawCall::DRAW_TEXT, left, top, fontSize, text);
    }

    const std::vector<DrawCall>& GetCalls() const
    {
        return m_calls;
    }

    void Clear()
    {
        m_calls.clear();
    }

    size_t GetCallCount() const
    {
        return m_calls.size();
    }

private:
    std::vector<DrawCall> m_calls;
};

} // namespace gfx