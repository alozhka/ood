cmake_minimum_required(VERSION 4.0)

# Включаем поддержку тестирования
enable_testing()

# Находим Qt Test модуль
find_package(Qt6 COMPONENTS Test REQUIRED)

# Создаем исполняемый файл для тестов
add_executable(GeometryDrawerTests
        GeometryDrawerTests.cpp
        ../src/model/Document.h
        ../src/model/Shape.h
        ../src/presenter/Commands.h
        ../src/presenter/JsonDocumentRepository.h
)

# Подключаем библиотеки Qt
target_link_libraries(GeometryDrawerTests
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Test
)

# Включаем автоматическую обработку MOC (Meta Object Compiler)
set_target_properties(GeometryDrawerTests PROPERTIES
        AUTOMOC ON
)

# Добавляем тест в CTest
add_test(NAME GeometryDrawerTests COMMAND GeometryDrawerTests)

# Устанавливаем рабочую директорию для тестов
set_tests_properties(GeometryDrawerTests PROPERTIES
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)