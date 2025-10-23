# uChmViewer build script with cmake
#
#       Common option:
# USE_QT5            - default enabled; if disabled, build with Qt6 instead.
# USE_KF5            - if defined, build with KDE Framework 5.
# USE_STATIC_CHMLIB  - if defined, static linkage to the chm library is used.
# USE_WEBKIT         - if defined, WebKit is used, otherwise WebEngine is used.
# USE_GETTEXT        - if defined, then GNU Gettext will be used for translation.
#                      The default is ON.
# USE_DBUS           - if defined, build with D-Bus integration.
#                      The default is ON for Linux.
# USE_MAC_APP        - if defined, build with  derived QApplication.
#                      The default is ON for macOs.
#
#       System-specific option:
# USE_DEPLOY_RUNTIME - if defined, the runtime dependencies will be copied to the
#                      folder with the executable. This option is applicable for
#                      Windows and macOS.
# USE_MACOS_BUNDLE   - if defined, configure installation for macOS as a bundle.
#                      The default is ON.
################################################

cmake_minimum_required(VERSION 3.16)

# Project info
project(uchmviewer HOMEPAGE_URL "https://github.com/eBookProjects/uChmViewer")
include(cmake/get-version.cmake)

include(cmake/use-in.cmake)
option(USE_QT5            "Use Qt5"                                          ON)
if (USE_QT5)
    option(USE_KF5            "Use KDE Framework 5"                         OFF)
    option(USE_WEBKIT         "Use Qt WebKit"                               OFF)
endif ()
option(USE_STATIC_CHMLIB  "Use static link with chm library"                OFF)
option(USE_DEPLOY_RUNTIME "Copy runtime dependencies for deployment"        OFF)
option(USE_GETTEXT        "Use GNU Gettext for translation"                  ON)
option(USE_MACOS_BUNDLE   "Install as macOS bundle"                          ON)
use_in(USE_DBUS           "Use D-Bus integration"                       "Linux")
use_in(USE_MAC_APP        "Use derived QApplication"                   "Darwin")

# Init cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Interface library to collect additional dependencies.
add_library(extra INTERFACE)

# Necessary packages
# libzip
find_package(libzip REQUIRED)

# Prepare to find Qt modules.
set(APP_QT_MIN_VERSION 5.9)
set(APP_QT_MODULES Core Network PrintSupport Widgets Xml)
if (USE_QT5)
    set(QT Qt5)
    if (USE_WEBKIT)
        set(APP_QT_MIN_VERSION 5.0)
        list(APPEND APP_QT_MODULES WebKit WebKitWidgets)
        target_link_libraries(extra INTERFACE Qt5::WebKit Qt5::WebKitWidgets)
        target_compile_definitions(extra INTERFACE USE_WEBKIT)
    endif ()
else ()
    set(QT Qt6)
    set(APP_QT_MIN_VERSION 6.0)
    # Since Qt6 QTextCodec moved in the Qt Core5Compat module.
    list(APPEND APP_QT_MODULES Core5Compat)
    target_link_libraries(extra INTERFACE Qt6::Core5Compat)
endif ()

if (NOT USE_WEBKIT)
    list(APPEND APP_QT_MODULES WebEngineWidgets)
    target_link_libraries(extra INTERFACE Qt::WebEngineWidgets)
endif ()

if (USE_DBUS)
    list(APPEND APP_QT_MODULES DBus)
    target_link_libraries(extra INTERFACE Qt::DBus)
    target_compile_definitions(extra INTERFACE USE_DBUS)
endif ()

if (USE_MAC_APP)
    target_compile_definitions(extra INTERFACE USE_MAC_APP)
endif ()

# Now we know all the required Qt modules.
find_package(${QT} ${APP_QT_MIN_VERSION} REQUIRED ${APP_QT_MODULES})

if (USE_QT5 AND USE_KF5)
    # Needed by find_package(KF5) below.
    find_package(ECM REQUIRED)
    list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
    find_package(KF5 REQUIRED KDELibs4Support)
    target_link_libraries(extra INTERFACE KF5::KDELibs4Support)
    target_compile_definitions(extra INTERFACE USE_KDE)
endif ()

include(cmake/qt-aliases.cmake) # only used for Qt <5.15
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
