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

cmake_minimum_required(VERSION 3.16)

############################
#  Core macros             #
############################

if (NOT Qt5_FOUND OR Qt5_VERSION VERSION_GREATER_EQUAL 5.15)
    return()
endif ()

macro(qt_wrap_cpp)
    qt5_wrap_cpp(${ARGV})
endmacro()

macro(qt_add_resources)
    qt5_add_resources(${ARGV})
endmacro()

macro(qt_generate_moc)
    qt5_generate_moc(${ARGV})
endmacro()

############################
#  Widgets macros          #
############################
macro(qt_wrap_ui outfile)
    qt5_wrap_ui(${ARGV})
endmacro()

############################
#  DBUS macros             #
############################
macro(qt_add_dbus_interface)
    qt5_add_dbus_interface(${ARGV})
endmacro()

macro(qt_add_dbus_interfaces)
    qt5_add_dbus_interfaces(${ARGV})
endmacro()

macro(qt_add_dbus_adaptor)
    qt5_add_dbus_adaptor(${ARGV})
endmacro()

macro(qt_generate_dbus_interface)
    qt5_generate_dbus_interface(${ARGV})
endmacro()

############################
#  Linguist tools macros   #
############################

macro(qt_create_translation)
    qt5_create_translation(${ARGV})
endmacro()

macro(qt_add_translation)
    qt5_add_translation(${ARGV})
endmacro()

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

if (USE_WEBKIT)
    qt_aliase(WebKit)
    qt_aliase(WebKitWidgets)
else ()
    qt_aliase(WebEngine)
    qt_aliase(WebEngineWidgets)
endif ()
