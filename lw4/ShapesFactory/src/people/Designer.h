#pragma once
#include "../shapes/IShapeFactory.h"
#include "IDesigner.h"

#include <iostream>

class Designer : public IDesigner
{
public:
	explicit Designer(IShapeFactory& factory)
		: m_factory(factory)
	{
	}

	PictureDraft CreateDraft(std::istream& input) override
	{
		PictureDraft draft;
		std::string line;

		while (std::getline(input, line))
		{
			try
			{
				std::unique_ptr<Shape> shape = m_factory.CreateShape(line);
				draft.AddShape(std::move(shape));
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}

		return draft;
	}

private:
	IShapeFactory& m_factory;
};