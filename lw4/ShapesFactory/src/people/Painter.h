#pragma once
#include "../canvas/ICanvas.h"
#include "../picture/PictureDraft.h"

class Painter
{
public:
	void DrawPicture(const PictureDraft& draft, ICanvas& canvas)
	{
		for (const auto& shape : draft)
		{
			shape->Draw(canvas);
		}
	}
};