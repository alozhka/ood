cmake_minimum_required(VERSION 3.28)
project(SimUDuck)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
        lib/Duck/Quack/SqueakBehavior.h
        lib/Duck/Dance/IDanceBehavior.h
        lib/Duck/Dance/WaltzDance.h
        lib/Duck/Dance/MinuetDance.h
        lib/Duck/Dance/NoDance.h)

enable_testing()

add_executable(DanceTests tests/DanceTests.cpp
        tests/MockDanceBehavior.h
        tests/TestDuck.h
        lib/Duck/Duck.h
        lib/Duck/Fly/FlyNoWay.h
        lib/Duck/Fly/IFlyBehavior.h
        lib/Duck/Quack/IQuakBehavior.h
        lib/Duck/Quack/MuteQuackBehavior.h
        lib/Duck/Dance/IDanceBehavior.h)

target_link_libraries(DanceTests 
  gtest_main
  gtest
)

include(GoogleTest)
gtest_discover_tests(DanceTests)
