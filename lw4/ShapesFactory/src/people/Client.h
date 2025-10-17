#pragma once
#include "IDesigner.h"
#include "Painter.h"

class Client
{
public:
	explicit Client(IDesigner& designer)
		: m_designer(designer)
	{
	}

	void HandleCommand(std::istream& input, ICanvas& canvas, Painter& painter)
	{
		PictureDraft draft = m_designer.CreateDraft(input);
		painter.DrawPicture(draft, canvas);
	}

private:
	IDesigner& m_designer;
};