#pragma once
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

	// Возвращает указатель на параграф, либо nullptr, если элемент не является параграфом
	std::shared_ptr<IParagraph> GetParagraph() const
	{
		return m_paragraph;
	}

	// TODO: добавить поддержку изображений
	// std::shared_ptr<IImage> GetImage() const;

private:
	std::shared_ptr<IParagraph> m_paragraph;
	// TODO: std::shared_ptr<IImage> m_image;
};