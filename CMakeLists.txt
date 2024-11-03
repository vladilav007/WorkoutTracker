cmake_minimum_required(VERSION 3.16)
project(WorkoutTracker VERSION 1.0 LANGUAGES CXX)

# Qt path can be set via command line: cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt
if(NOT CMAKE_PREFIX_PATH)
    if(EXISTS $ENV{HOME}/Qt)
        file(GLOB QT_VERSIONS "$ENV{HOME}/Qt/[0-9]*.[0-9]*.[0-9]*")
        if(QT_VERSIONS)
            list(GET QT_VERSIONS -1 LATEST_QT)
            set(CMAKE_PREFIX_PATH "${LATEST_QT}/gcc_64")
        endif()
    endif()
endif()

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
    src/views/customcalendarwidget.cpp
    src/views/workoutdialog.cpp
)

set(HEADERS
    src/views/mainwindow.h
    src/views/customcalendarwidget.h
    src/views/workoutdialog.h
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