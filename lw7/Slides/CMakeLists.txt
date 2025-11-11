cmake_minimum_required(VERSION 4.0)
project(Slides)

set(CMAKE_CXX_STANDARD 20)

add_executable(Slides main.cpp
        src/parse/CommandController.h
        src/parse/Menu.h
        src/slide/ISlide.h
        src/slide/Slide.h
        src/shape/Shape.h
        src/shape/factory/ShapesFactory.h
        src/shape/types/Rectangle.h
        src/shape/Point.h
        src/shape/Style.h
        src/shape/TemplateFrame.h
        src/canvas/ICanvas.h)


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

add_executable(SlidesTests tests/SlidesTests.cpp)

target_link_libraries(SlidesTests
        gtest_main
        gtest
)

include(GoogleTest)
gtest_discover_tests(SlidesTests)