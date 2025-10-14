cmake_minimum_required(VERSION 4.0)
project(WeatherStationSignals)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        Boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BOOST_INCLUDE_LIBRARIES signals2)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(Boost)

add_executable(WeatherStation main.cpp
        src/WeatherData.h
        src/Observer.h
        src/Display.h
        src/StatsInfo.h
)

target_link_libraries(WeatherStation PRIVATE Boost::signals2)

# Fetch GoogleTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(WeatherStationTests tests/WeaterStationTests.cpp)

enable_testing()

target_link_libraries(WeatherStationTests
        gtest_main
        gtest
        Boost::signals2
)

include(GoogleTest)
gtest_discover_tests(WeatherStationTests)
