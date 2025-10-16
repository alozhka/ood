cmake_minimum_required(VERSION 4.0)
project(streams)

set(CMAKE_CXX_STANDARD 20)

add_executable(streams main.cpp
        src/output/decorators/CompressionOutputStream.h
        src/input/decorators/DecompressInputStream.h)

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

add_executable(StreamsTests
        tests/StreamsTests.cpp
        src/parse/CommandLineParser.cpp
        src/parse/CommandLineParser.h
        src/output/IOutputDataStream.h
        src/output/MemoryOutputStream.h
        src/output/MemoryOutputStream.cpp
        src/input/IInputDataStream.h
        src/input/MemoryInputStream.h
        src/input/MemoryInputStream.cpp
        src/output/decorators/OutputStreamDecorator.h
        src/input/decorators/InputStreamDecorator.h
        src/output/decorators/EncryptionTable.h
        src/output/decorators/EncryptionOutputStream.h
        src/input/decorators/DecryptionInputStream.h)

target_link_libraries(StreamsTests
        gtest_main
        gtest
)

include(GoogleTest)
gtest_discover_tests(StreamsTests)