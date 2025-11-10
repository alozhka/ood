#pragma once
#include "../IShape.h"

class IShapesFactory
{
public:
    virtual std::shared_ptr<IShape> Create(const std::string& type, std::istream& params) = 0;

    virtual ~IShapesFactory() = default;
};
