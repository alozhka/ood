cmake_minimum_required(VERSION 4.0)
project(WeatherStation)

set(CMAKE_CXX_STANDARD 20)

add_executable(WeatherStation main.cpp
        src/WeatherData.h
        src/Observer.h
        src/Display.h)
