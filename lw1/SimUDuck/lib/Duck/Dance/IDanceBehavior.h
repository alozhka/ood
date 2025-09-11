#pragma once

class IDanceBehavior
{
public:
	virtual ~IDanceBehavior() = default;
	virtual void Dance() const = 0;
};
