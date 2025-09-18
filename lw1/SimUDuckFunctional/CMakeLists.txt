cmake_minimum_required(VERSION 3.28)
project(SimUDuckFunctional)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(SimUDuckFunctional main.cpp
        lib/DuckFunctions.h
        lib/Duck/Duck.h
        lib/Duck/MallardDuck.h
        lib/Duck/RedheadDuck.h
        lib/Duck/RubberDuck.h
        lib/Duck/DecoyDuck.h
        lib/Duck/ModelDuck.h
        lib/Duck/Fly/FlyBehaviorFunctions.h
        lib/Duck/Dance/DanceBehaviorFunctions.h
        lib/Duck/Quack/QuackBehaviorFunctions.h
        lib/Duck/Quack/QuackBehaviorFunctions.h
)