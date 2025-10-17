#pragma once
#include "../picture/PictureDraft.h"
#include <istream>

class IDesigner
{
public:
	virtual PictureDraft CreateDraft(std::istream& input) = 0;
	virtual ~IDesigner() = default;
};