cmake_minimum_required(VERSION 4.0)
project(HtmlEditor)

set(CMAKE_CXX_STANDARD 20)

add_executable(HtmlEditor main.cpp
        src/Menu.h
        src/CommandController.h
        src/Document/IDocument.h
        src/Document/Document.h
)

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

add_executable(HtmlEditorTests tests/HtmlEditorTests.cpp)
target_link_libraries(HtmlEditorTests
        gtest_main
        gtest
)
include(GoogleTest)
gtest_discover_tests(HtmlEditorTests)