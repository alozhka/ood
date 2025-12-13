cmake_minimum_required(VERSION 4.0)
project(GeometryDrawer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Qml
        REQUIRED)

add_executable(GeometryDrawer main.cpp
        src/view/MainWindow.h
        src/view/MainWindow.cpp
        src/presenter/DocumentPresenter.cpp
        src/presenter/DocumentPresenter.h
        src/presenter/CanvasPresenter.cpp
        src/presenter/CanvasPresenter.h
)

target_link_libraries(GeometryDrawer
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Qml
)

