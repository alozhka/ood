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
        REQUIRED)

add_executable(GeometryDrawer main.cpp
        src/MainWindow.h
        src/MainWindow.cpp
        src/presenter/DocumentPresenter.h
        src/view/ShapeView.h
        src/view/RectangleView.h
        src/model/Document.h
        src/model/Shape.h
        src/view/TriangleView.h
        src/view/EllipseView.h
        src/view/IResizable.h
)

target_link_libraries(GeometryDrawer
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

