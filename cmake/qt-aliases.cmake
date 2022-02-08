################################################################################
# Copyright (C) 2021  Nick Egorrov
# License: MIT https://mit-license.org/
#
# A set of aliases for Qt4 and Qt5 functions to facilitate assembly with
# different versions. Since Qt5.15 there are similar (and some new) aliases
# in Qt, in which case aliases not be defined here.
#
#  Core macros
#    qt_wrap_cpp()
#    qt_add_resources()
#    qt_generate_moc()
#
#  Widgets macros
#    qt_wrap_ui()
#
#  DBUS macros
#    qt_add_dbus_interface()
#    qt_add_dbus_interfaces()
#    qt_add_dbus_adaptor()
#    qt_generate_dbus_interface()
#
#  Linguist tools macros
#    qt_create_translation()
#    qt_add_translation()
#
# IMPORTANT! This file must be included after finding Qt.
#
# find_package(Qt4 REQUIRED ...)
# include(qt-aliases.cmake)
#
################################################################################

cmake_minimum_required(VERSION 3.13)

############################
#  Core macros             #
############################

# qt_wrap_cpp(outfiles)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_wrap_cpp)
        qt5_wrap_cpp(${ARGV})
    endmacro()
endif ()

# qt_add_resources(outfiles)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_add_resources)
        qt5_add_resources(${ARGV})
    endmacro()
endif ()

# qt_generate_moc()
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_generate_moc)
        qt5_generate_moc(${ARGV})
    endmacro()
endif ()


############################
#  Widgets macros          #
############################

# qt_wrap_ui(outfiles)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_wrap_ui outfile)
        qt5_wrap_ui(${ARGV})
    endmacro()
endif ()


############################
#  DBUS macros             #
############################

# qt_add_dbus_interface(outfiles)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_add_dbus_interface)
        qt5_add_dbus_interface(${ARGV})
    endmacro()
endif ()

# qt_add_dbus_interfaces(outfiles)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_add_dbus_interfaces)
        qt5_add_dbus_interfaces(${ARGV})
    endmacro()
endif()

# qt_add_dbus_adaptor(outfiles)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_add_dbus_adaptor)
        qt5_add_dbus_adaptor(${ARGV})
    endmacro()
endif ()

# qt_generate_dbus_interface()
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_generate_dbus_interface)
        qt5_generate_dbus_interface(${ARGV})
    endmacro()
endif ()


############################
#  Linguist tools macros   #
############################

# qt_create_translation(qm_files)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_create_translation)
        qt5_create_translation(${ARGV})
    endmacro()
endif ()

# qt_add_translation(qm_files)
if (QT_VERSION_MAJOR VERSION_LESS 5.15)
    macro(qt_add_translation)
        qt5_add_translation(${ARGV})
    endmacro()
endif ()


############################
#  Module aliases          #
############################

macro(qt_aliase)
    if(NOT TARGET Qt::${ARGV0})
		add_library(Qt::${ARGV0} INTERFACE IMPORTED)
		target_link_libraries(Qt::${ARGV0} INTERFACE Qt5::${ARGV0})
    endif()
endmacro()

qt_aliase(Core)
qt_aliase(DBus)
qt_aliase(Network)
qt_aliase(PrintSupport)
qt_aliase(Widgets)
qt_aliase(Xml)

if (${USE_WEBENGINE})
    qt_aliase(WebEngine)
    qt_aliase(WebEngineWidgets)
else ()
    qt_aliase(WebKit)
    qt_aliase(WebKitWidgets)
endif ()
