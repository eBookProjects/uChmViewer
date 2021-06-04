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


############################
#  Core macros             #
############################

# qt_wrap_cpp(outfiles)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_wrap_cpp)
        if (QT4_FOUND)
            qt4_wrap_cpp(${ARGV})
        else ()
            qt5_wrap_cpp(${ARGV})
        endif ()
    endmacro()
endif()

# qt_add_resources(outfiles)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_add_resources)
        if (QT4_FOUND)
            qt4_add_resources(${ARGV})
        else ()
            qt5_add_resources(${ARGV})
        endif ()
    endmacro()
endif()

# qt_generate_moc()
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_generate_moc)
        if (QT4_FOUND)
            qt4_generate_moc(${ARGV})
        else ()
            qt5_generate_moc(${ARGV})
        endif ()
    endmacro()
endif ()


############################
#  Widgets macros          #
############################

# qt_wrap_ui(outfiles)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_wrap_ui outfile)
        if (QT4_FOUND)
            qt4_wrap_ui(${ARGV})
        else ()
            qt5_wrap_ui(${ARGV})
        endif ()
    endmacro()
endif()


############################
#  DBUS macros             #
############################

# qt_add_dbus_interface(outfiles)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_add_dbus_interface)
        if (QT4_FOUND)
            qt4_add_dbus_interface(${ARGV})
        else ()
            qt5_add_dbus_interface(${ARGV})
        endif ()
    endmacro()
endif()

# qt_add_dbus_interfaces(outfiles)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_add_dbus_interfaces)
        if (QT4_FOUND)
            qt4_add_dbus_interfaces(${ARGV})
        else ()
            qt5_add_dbus_interfaces(${ARGV})
        endif ()
    endmacro()
endif()

# qt_add_dbus_adaptor(outfiles)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_add_dbus_adaptor)
        if (QT4_FOUND)
            qt4_add_dbus_adaptor(${ARGV})
        else ()
            qt5_add_dbus_adaptor(${ARGV})
        endif ()
    endmacro()
endif()

# qt_generate_dbus_interface()
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_generate_dbus_interface)
        if (QT4_FOUND)
            qt4_generate_dbus_interface(${ARGV})
        else ()
            qt5_generate_dbus_interface(${ARGV})
        endif ()
    endmacro()
endif()


############################
#  Linguist tools macros   #
############################

# qt_create_translation(qm_files)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_create_translation)
        if (QT4_FOUND)
            qt4_create_translation(${ARGV})
        else ()
            qt5_create_translation(${ARGV})
        endif ()
    endmacro()
endif()

# qt_add_translation(qm_files)
if (NOT QT_NO_CREATE_VERSIONLESS_FUNCTIONS)
    macro(qt_add_translation)
        if (QT4_FOUND)
            qt4_add_translation(${ARGV})
        else ()
            qt5_add_translation(${ARGV})
        endif ()
    endmacro()
endif()


############################
#  Module aliases          #
############################

macro(qt_aliase)
    if(NOT TARGET Qt::${ARGV0})
	add_library(Qt::${ARGV0} INTERFACE IMPORTED)
	if (QT4_FOUND)
	    target_link_libraries(Qt::${ARGV0} INTERFACE Qt4::${ARGV1})
	else ()
	    target_link_libraries(Qt::${ARGV0} INTERFACE Qt5::${ARGV0})
	endif ()
    endif()
endmacro()

qt_aliase(Core QtCore)
qt_aliase(DBus QtDBus)
qt_aliase(Network QtNetwork)
qt_aliase(PrintSupport QtGui)
qt_aliase(Widgets QtGui)
qt_aliase(Xml QtXml)

if (${QT_USE_WEBENGINE})
    qt_aliase(WebEngine WebEngine)
    qt_aliase(WebEngineWidgets WebEngineWidgets)
else ()
    qt_aliase(WebKit QtWebKit)
    qt_aliase(WebKitWidgets QtWebKit)
endif ()
