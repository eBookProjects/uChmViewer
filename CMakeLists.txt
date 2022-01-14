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

option(USE_KF5            "Use KDE Framework 5"                             OFF)
option(USE_WEBENGINE      "Use Qt WebEngine"                                OFF)
option(USE_STATIC_CHMLIB  "Use static link with chm library"                OFF)
option(USE_DEPLOY_RUNTIME "Copying runtime dependencies for deployment"     OFF)
option(USE_GETTEXT        "Use GNU Gettext for translation"                  ON)
option(USE_MACOS_BUNDLE   "Install as macOS bundle"                          ON)

# Init cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Necessary packages
# libzip
find_package(libzip REQUIRED)

set(APP_QT_VERSION 5.0)
set(APP_QT_PACKAGES Core DBus Network PrintSupport Widgets Xml)

if (${USE_WEBENGINE})
    set(APP_QT_VERSION 5.9)
    list(APPEND APP_QT_PACKAGES WebEngine WebEngineWidgets)
    add_definitions(-DUSE_WEBENGINE)
else ()
    list(APPEND APP_QT_PACKAGES WebKit WebKitWidgets)
endif ()

find_package(Qt5 ${APP_QT_VERSION} REQUIRED ${APP_QT_PACKAGES})

if (${USE_KF5})
    # Needed by find_package(KF5) below.
    find_package(ECM REQUIRED)
    list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
    find_package(KF5 COMPONENTS KDELibs4Support)
    
    add_definitions(-DUSE_KDE)
    add_definitions(${KDE4_DEFINITIONS})
    include_directories(${KDE4_INCLUDES})
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
