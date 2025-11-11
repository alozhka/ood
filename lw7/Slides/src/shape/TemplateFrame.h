#pragma once

template <typename T>
struct TemplateFrame
{
	T left, top, width, height;
};

using Frame = TemplateFrame<double>;
