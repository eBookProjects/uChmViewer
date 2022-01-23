# uChmViewer build script with cmake
#
#       Common option:
# USE_KF5            - if defined, build with KDE Framework 5.
# USE_STATIC_CHMLIB  - if defined, static linkage to the chm library is used.
# USE_WEBENGINE      - if defined, WebEngine is used, otherwise WebKit is used.
# USE_GETTEXT        - if defined, then GNU Gettext will be used for translation.
#                      The default is ON.
#
#       System-specific option:
# USE_DEPLOY_RUNTIME - if defined, the runtime dependencies will be copied to the
#                      folder with the executable. This option is applicable for
#                      Windows and macOS.
# USE_MACOS_BUNDLE   - if defined, configure installation for macOS as a bundle.
#                      The default is ON.
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
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Interface library to collect additional dependencies.
add_library(extra INTERFACE)

# Necessary packages
# libzip
find_package(libzip REQUIRED)

# Prepare to find Qt modules.
set(APP_QT_VERSION 5.0)
set(APP_QT_MODULES Core DBus Network PrintSupport Widgets Xml)
# Modules for WebKit or WebEngine.
if (${USE_WEBENGINE})
    set(APP_QT_VERSION 5.9)
    list(APPEND APP_QT_MODULES WebEngine WebEngineWidgets)
    target_link_libraries(extra INTERFACE Qt::WebEngine Qt::WebEngineWidgets)
    target_compile_definitions(extra INTERFACE USE_WEBENGINE)
else ()
    list(APPEND APP_QT_MODULES WebKit WebKitWidgets)
    target_link_libraries(extra INTERFACE Qt::WebKit Qt::WebKitWidgets)
endif ()
# What version of Qt are we working with?
find_package(QT ${APP_QT_VERSION} NAMES Qt5 Qt6 REQUIRED)
set(QT Qt${QT_VERSION_MAJOR})
# Since Qt 6 QTextCodec moved in the Qt 5 Core Compat module.
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    list(APPEND APP_QT_MODULES Core5Compat)
    target_link_libraries(extra INTERFACE ${QT}::Core5Compat)
endif ()
# Now we know all the required Qt modules.
find_package(${QT} REQUIRED ${APP_QT_MODULES})

if (${USE_KF5})
    # Needed by find_package(KF5) below.
    find_package(ECM REQUIRED)
    list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
    find_package(KF5 REQUIRED KDELibs4Support)
    target_link_libraries(extra INTERFACE KF5::KDELibs4Support)
    target_compile_definitions(extra INTERFACE USE_KDE)
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
