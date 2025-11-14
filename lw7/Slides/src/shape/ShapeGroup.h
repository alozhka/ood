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
		if (m_shapes.empty())
		{
			return;
		}
		std::optional<Frame> groupFrame = GetFrame();
		if (!groupFrame)
		{
			return;
		}

		double widthScale = rect.width / groupFrame->width;
		double heightScale = rect.height / groupFrame->height;
		for (const auto& shape : m_shapes)
		{
			std::optional<Frame> shapeFrame = shape->GetFrame();

			double leftOffset = shapeFrame->left - groupFrame->left;
			double scaledLeft = widthScale * leftOffset;
			double topOffset = shapeFrame->top - groupFrame->top;
			double scaledTop = heightScale * topOffset;

			shape->SetFrame(
				{ rect.left + scaledLeft,
					rect.top + scaledTop,
					shapeFrame->width * widthScale,
					shapeFrame->height * heightScale });
		}
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

	std::shared_ptr<IShape> Clone() const override
	{
		std::vector<std::shared_ptr<IShape>> clonedShapes;
		clonedShapes.reserve(m_shapes.size());

		for (const auto& shape : m_shapes)
		{
			clonedShapes.push_back(shape->Clone());
		}

		return std::make_shared<ShapeGroup>(clonedShapes);
	}

	void Draw(ICanvas& canvas) const override
	{
		for (const auto& shape : m_shapes)
		{
			shape->Draw(canvas);
		}
	}

private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
};