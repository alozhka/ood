#pragma once


class IShape
{
public:
    virtual std::string GetType() const = 0;
    virtual std::string ListParams() const = 0;

    virtual ~IShape() = default;
};
