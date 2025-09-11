cmake_minimum_required(VERSION 3.28)
project(SimUDuck)

set(CMAKE_CXX_STANDARD 23)

add_executable(SimUDuck main.cpp
        lib/DuckFunctions.h
        lib/Duck/RubberDuck.h
        lib/Duck/RedheadDuck.h
        lib/Duck/DecoyDuck.h
        lib/Duck/Duck.h
        lib/Duck/MallardDuck.h
        lib/Duck/ModelDuck.h
        lib/Duck/Fly/FlyNoWay.h
        lib/Duck/Fly/FlyWithWings.h
        lib/Duck/Fly/IFlyBehavior.h
        lib/Duck/Quack/IQuakBehavior.h
        lib/Duck/Quack/MuteQuackBehavior.h
        lib/Duck/Quack/QuackBehavior.h
        lib/Duck/Quack/SqueakBehavior.h)
