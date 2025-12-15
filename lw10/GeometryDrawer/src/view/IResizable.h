#pragma once

class IResizable
{
public:
	virtual QRectF GetRect() const = 0;
	virtual void SetRect(const QRectF& rect) = 0;
	virtual ~IResizable() = default;
};