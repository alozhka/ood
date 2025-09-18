#pragma once

#include "../lib/Duck/Duck.h"
#include "../lib/Duck/Quack/QuackBehavior.h"

class MockDuck : public Duck {
public:
    explicit MockDuck(std::unique_ptr<IDanceBehavior> &&danceBehavior)
        : Duck(
            std::make_unique<FlyNoWay>(),
            std::make_unique<QuackBehavior>(),
            std::move(danceBehavior)
        ) {
    }

    void Display() const override {
        std::cout << "Mock duck";
    }
};
