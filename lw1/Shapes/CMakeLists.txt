cmake_minimum_required(VERSION 4.0)
project(Shapes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOC OFF)
set(SFML_BUILD_TESTS OFF)

FetchContent_MakeAvailable(SFML)

add_executable(Shapes main.cpp
        src/Draw/ICanvas.h
        src/Draw/SFMLCanvas.cpp
        src/Draw/SFMLCanvas.h
        src/commands/ShapesController.cpp
        src/commands/ShapesController.h
        src/shapes/Picture.cpp
        src/shapes/Picture.h
        src/commands/ShapesFactory.cpp
        src/commands/ShapesFactory.h
        src/shapes/strategy/IShapeStrategy.h
        src/shapes/Shape.cpp
        src/shapes/Shape.h
        src/shapes/strategy/ShapesStrategies.h
        src/shapes/Point.h)

target_link_libraries(Shapes SFML::Graphics)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(ShapesTests tests/ShapesTests.cpp src/shapes/Shape.cpp src/commands/ShapesFactory.cpp)

target_link_libraries(ShapesTests
        gtest_main
        gtest
)

include(GoogleTest)
gtest_discover_tests(ShapesTests)