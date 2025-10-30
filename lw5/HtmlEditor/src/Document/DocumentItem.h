#pragma once
#include "IImage.h"
#include "IParagraph.h"
#include <memory>

/*
Элемент документа. Позволяет получить доступ к изображению или параграфу
*/
class DocumentItem
{
public:
	explicit DocumentItem(std::shared_ptr<IParagraph> paragraph)
		: m_paragraph(std::move(paragraph))
	{
	}

	explicit DocumentItem(std::shared_ptr<IImage> image)
		: m_image(std::move(image))
	{
	}

	// Возвращает указатель на параграф, либо nullptr, если элемент не является параграфом
	std::shared_ptr<IParagraph> GetParagraph() const
	{
		return m_paragraph;
	}

	// Возвращает указатель на изображение, либо nullptr, если элемент не является изображением
	std::shared_ptr<IImage> GetImage() const
	{
		return m_image;
	}

private:
	std::shared_ptr<IParagraph> m_paragraph;
	std::shared_ptr<IImage> m_image;
};