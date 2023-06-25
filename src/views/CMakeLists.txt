# author: Niklas Hempel
# mail: s85509@htw-dresden.de
# group: 22/041/65

cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(
    views
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(views PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(views PROPERTIES

    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com

    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)
