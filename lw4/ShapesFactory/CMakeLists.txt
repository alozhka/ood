cmake_minimum_required(VERSION 4.0)
project(ShapesFactory)

set(CMAKE_CXX_STANDARD 20)

add_executable(ShapesFactory main.cpp
        src/canvas/ICanvas.h
        src/canvas/Color.h
        src/canvas/Point.h
        src/canvas/SVGCanvas.h)

# TESTS
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        GIT_SHALLOW ON
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(ShapesFactoryTests
        tests/TestCanvas.h)

target_link_libraries(ShapesFactoryTests
        gtest_main
        gtest
)

include(GoogleTest)
gtest_discover_tests(ShapesFactoryTests)