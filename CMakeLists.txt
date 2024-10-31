# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(WorkoutTracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    Widgets
)

# Project structure
set(SOURCES
    src/main.cpp
    src/views/mainwindow.cpp
)

set(HEADERS
    src/views/mainwindow.h
)

add_executable(WorkoutTracker
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(WorkoutTracker PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Include directories
target_include_directories(WorkoutTracker PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)