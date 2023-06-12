cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(
    mylib
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
    qt_add_executable(mylib

        MANUAL_FINALIZATION

        ${PROJECT_SOURCES}
    )

else()
    add_executable(mylib

        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(mylib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(mylib PROPERTIES

    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com

    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}

    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}

    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)

install(TARGETS mylib

    BUNDLE DESTINATION .

    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mylib)
endif()
