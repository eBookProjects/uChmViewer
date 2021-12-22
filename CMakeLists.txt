# uChmViewer build script with cmake
#
#       Common option:
# FRAMEWORK=<Qt4 | Qt5 | KDE4>
# USE_STATIC_CHMLIB=ON - if defined, static linkage to the chm library is used
# USE_WEBENGINE=ON - if defined, WebEngine is used, otherwise WebKit is used
# USE_GETTEXT=ON - if defined, then GNU Gettext will be used for translation.
#     The default is ON.
#
#       System-specific option:
#
# USE_DEPLOY_RUNTIME=ON - if defined, the runtime dependencies will be copied to
#     the folder with the executable. This option is applicable for Windows and
#     macOS.
# USE_MACOS_BUNDLE=ON - if defined, configure installation for macOS as a bundle.
#     The default is ON.
################################################

cmake_minimum_required(VERSION 3.13)

# Project info
project(uchmviewer HOMEPAGE_URL "https://github.com/u-235/uchmviewer")
include(cmake/get-version.cmake)
set(CMAKE_PROJECT_VERSION ${PROJECT_VERSION})

option(USE_WEBENGINE      "Use Qt WebEngine"                                OFF)
option(USE_STATIC_CHMLIB  "Use static link with chm library"                OFF)
option(USE_DEPLOY_RUNTIME "Copying runtime dependencies for deployment"     OFF)
option(USE_GETTEXT        "Use GNU Gettext for translation"                  ON)
option(USE_MACOS_BUNDLE   "Install as macOS bundle"                          ON)

# Init cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if (NOT DEFINED FRAMEWORK)
    set(FRAMEWORK Qt5)
    message(STATUS "Choice <${FRAMEWORK}> as default framework.")
endif ()

# Necessary packages
# libzip
find_package(libzip REQUIRED)

if (${FRAMEWORK} STREQUAL Qt4)
    find_package(Qt4 REQUIRED QtCore QtDBus QtNetwork QtWebKit QtGui QtXml)
elseif (${FRAMEWORK} STREQUAL Qt5)
    find_package(Qt5 REQUIRED Core DBus Network PrintSupport Widgets Xml)

    if (${USE_WEBENGINE})
        find_package(Qt5 5.9 REQUIRED WebEngine WebEngineWidgets)
    else ()
        find_package(Qt5 REQUIRED WebKit WebKitWidgets)
    endif ()
elseif (${FRAMEWORK} STREQUAL KDE4)
    find_package(KDE4 REQUIRED)
    add_definitions(${KDE4_DEFINITIONS})
    add_definitions(-DUSE_KDE)
    include_directories(${KDE4_INCLUDES})
else ()
    message(SEND_ERROR "Unknown framework <${FRAMEWORK}>")
endif ()

if (${USE_WEBENGINE})
    add_definitions(-DUSE_WEBENGINE)
endif ()

include(cmake/qt-aliases.cmake)
include(cmake/install-helper.cmake)

add_compile_options(
    $<$<CXX_COMPILER_ID:MSVC>:/W3>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wextra>)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 11)

# Run in those subdirectories
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(packages)
