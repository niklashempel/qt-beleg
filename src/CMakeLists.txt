cmake_minimum_required(VERSION 3.5)

project(qt-beleg VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    person.hpp
    person.cpp
    medium.hpp
    medium.cpp
    book.hpp
    cd.hpp
    dvd.hpp
    list.hpp
    datastore.hpp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt-beleg

        MANUAL_FINALIZATION

        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(qt-beleg SHARED

            ${PROJECT_SOURCES}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:

    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qt-beleg

            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qt-beleg PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(qt-beleg PROPERTIES

    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com

    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)

install(TARGETS qt-beleg

    BUNDLE DESTINATION .

    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt-beleg)
endif()
