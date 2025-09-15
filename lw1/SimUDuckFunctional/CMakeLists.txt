cmake_minimum_required(VERSION 3.28)
project(SimUDuckFunctional)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
)

enable_testing()

add_executable(FunctionalDuckBehaviorTests tests/FunctionalDuckBehaviorTests.cpp
        lib/Duck/Duck.h
        lib/Duck/MallardDuck.h
        lib/Duck/RedheadDuck.h
        lib/Duck/RubberDuck.h
        lib/Duck/DecoyDuck.h
        lib/Duck/ModelDuck.h
        lib/Duck/Fly/FlyBehaviorFunctions.h
        lib/Duck/Dance/DanceBehaviorFunctions.h
        lib/Duck/Quack/QuackBehaviorFunctions.h)

target_link_libraries(FunctionalDuckBehaviorTests
        gtest_main
        gtest
)

include(GoogleTest)
gtest_discover_tests(FunctionalDuckBehaviorTests)
