#pragma once
#include "Style.h"
#include "TemplateFrame.h"

#include <string>

class IShape
{
public:
	virtual void SetFrame(const Frame& rect) = 0;
	virtual void SetLineStyle(RGBAColor color, bool isEnabled) = 0;
	virtual void SetFillStyle(RGBAColor color, bool isEnabled) = 0;

	// TODO: сделать рисование
	// virtual void Draw(ICanvas& canvas) const = 0;

	virtual std::optional<Frame> GetFrame() const = 0;
	virtual std::optional<Style> GetLineStyle() const = 0;
	virtual std::optional<Style> GetFillStyle() const = 0;
	virtual std::string GetType() const = 0;

	virtual ~IShape() = default;
};