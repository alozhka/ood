cmake_minimum_required(VERSION 4.0)
project(TileImage)

set(CMAKE_CXX_STANDARD 20)

add_executable(TileImage main.cpp
        src/tile.h
        src/cow.h
        src/image/ImageService.h
        src/drawer/drawer.cpp)

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

add_executable(TileImageTests tests/TileImageTests.cpp
        src/drawer/drawer.cpp)

target_link_libraries(TileImageTests
        gtest_main
        gtest
)

include(GoogleTest)
gtest_discover_tests(TileImageTests)
