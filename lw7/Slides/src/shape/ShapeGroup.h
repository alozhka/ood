#pragma once
#include "IShape.h"
#include <vector>

class ShapeGroup final : public IShape
{
public:
	explicit ShapeGroup(const std::vector<std::shared_ptr<IShape>>& shapes)
		: m_shapes(shapes)
	{
	}

	void SetFrame(const Frame& rect) override
	{
		throw std::logic_error("Not implemented yet!");
	}

	void SetLineStyle(RGBAColor color, bool isEnabled) override
	{
		for (const std::shared_ptr<IShape>& shape : m_shapes)
		{
			shape->SetLineStyle(color, isEnabled);
		}
	}

	void SetFillStyle(RGBAColor color, bool isEnabled) override
	{
		for (const std::shared_ptr<IShape>& shape : m_shapes)
		{
			shape->SetFillStyle(color, isEnabled);
		}
	}

	std::optional<Frame> GetFrame() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		double left = std::numeric_limits<double>::max();
		double bottom = std::numeric_limits<double>::lowest();
		double top = std::numeric_limits<double>::max();
		double right = std::numeric_limits<double>::lowest();

		for (const std::shared_ptr<IShape>& shape : m_shapes)
		{
			std::optional<Frame> frame = shape->GetFrame();
			if (!frame)
			{
				return std::nullopt;
			}

			left = std::min(left, frame->left);
			top = std::min(top, frame->top);
			right = std::max(right, frame->left + frame->width);
			bottom = std::max(bottom, frame->top + frame->height);
		}

		return Frame{ left, top, right - left, bottom - top };
	}

	std::optional<Style> GetLineStyle() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		std::optional<Style> firstStyle = m_shapes[0]->GetLineStyle();
		for (const std::shared_ptr<IShape>& shape : m_shapes)
		{
			std::optional<Style> style = shape->GetLineStyle();

			if (firstStyle->GetColor() != style->GetColor())
			{
				return std::nullopt;
			}
		}

		return firstStyle;
	}

	std::optional<Style> GetFillStyle() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		std::optional<Style> firstStyle = m_shapes[0]->GetFillStyle();
		for (const std::shared_ptr<IShape>& shape : m_shapes)
		{
			std::optional<Style> style = shape->GetFillStyle();
			if (firstStyle->GetColor() != style->GetColor())
			{
				return std::nullopt;
			}
		}

		return firstStyle;
	}

	std::string GetType() const override
	{
		return "group";
	}

private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
};