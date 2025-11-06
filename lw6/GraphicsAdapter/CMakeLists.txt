cmake_minimum_required(VERSION 4.0)
project(GraphicsAdapter)

set(CMAKE_CXX_STANDARD 20)

add_executable(GraphicsAdapter main.cpp
        src/graphics_lib.h
        src/shapes_drawing_lib.h
        src/modern_graphics_lib.h
        src/adapter/ModernToOldGraphicsAdapted.h)

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

add_executable(ObjectAdapterTests tests/ObjectAdapterTests.cpp)