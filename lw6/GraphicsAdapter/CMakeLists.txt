cmake_minimum_required(VERSION 4.0)
project(GraphicsAdapter)

set(CMAKE_CXX_STANDARD 20)

add_executable(GraphicsAdapter main.cpp
        src/graphics_lib.h
        src/shapes_drawing_lib.h
        src/modern_graphics_lib.h)
