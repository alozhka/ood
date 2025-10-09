cmake_minimum_required(VERSION 4.0)
project(WeatherStation)

set(CMAKE_CXX_STANDARD 20)

add_executable(WeatherStation main.cpp
        src/WeatherData.h
        src/Observer.h
        src/Display.h
        src/Observable.h
        src/StatsInfo.h)

add_executable(WeatherStationTests tests/WeaterStationTests.cpp
        tests/BrokenDisplay.h
        tests/OrderTestDisplay.h
        tests/MockDisplay.h)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(WeatherStationTests
        gtest_main
        gtest
)

include(GoogleTest)
gtest_discover_tests(WeatherStationTests)
