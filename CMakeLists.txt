#
# Option:
# -DQT4_ONLY=ON          build with Qt4, not use KDE
# -DQT5_ONLY=ON          build with Qt5, not use KDE
# -DQT_USE_WEBENGINE=ON  use Qt WebEngine
################################################

cmake_minimum_required(VERSION 3.0)

# Project name
project(kchmviewer)

option(QT4_ONLY         "Build with Qt4, not use KDE")
option(QT5_ONLY         "Build with Qt4, not use KDE")
option(QT_USE_WEBENGINE "Use Qt WebEngine")

# Init cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Necessary packages
# libzip
find_package(libzip REQUIRED)

if (${QT4_ONLY})
    find_package(Qt4 REQUIRED QtCore QtDBus QtNetwork QtWebKit QtGui QtXml)
elseif (${QT5_ONLY})
    find_package(Qt5 REQUIRED Core DBus Network PrintSupport Widgets Xml)
    if (${QT_USE_WEBENGINE})
        find_package(Qt5 5.9 REQUIRED WebEngine WebEngineWidgets)
    else ()
        find_package(Qt5 REQUIRED WebKit WebKitWidgets)
    endif ()
else ()
    find_package(KDE4 REQUIRED)
    add_definitions(${KDE4_DEFINITIONS})
    add_definitions(-DUSE_KDE)
    include_directories(${KDE4_INCLUDES})
endif ()

if (${QT_USE_WEBENGINE})
    add_definitions(-DUSE_WEBENGINE)
endif ()

include(cmake/qt-aliases.cmake)

add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/W3>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wextra>)

# Run in those subdirectories
add_subdirectory(lib)
#add_subdirectory(lib/libebook)
add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(packages)
