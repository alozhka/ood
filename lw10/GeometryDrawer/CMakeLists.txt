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
        src/view-model/GeometryViewModel.h)

qt_add_resources(GeometryDrawer "qml"
    PREFIX "/"
    FILES
    main.qml
    src/view/MenuBar.qml
    src/view/ToolBar.qml
    src/view/GeometryCanvas.qml
    src/view/LayersPanel.qml
    src/view/StatusBar.qml
)

target_link_libraries(GeometryDrawer
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Qml
)

