# The location of the installation directories. Only relative paths may be used.
#
# APP_BIN_INSTALL_DIR    installation folder of the executable.
# APP_DEF_INSTALL_DIR    installation folder of the desktop entry file.
# APP_ICONS_INSTALL_DIR  folder for installing application icons.
# APP_LOCALE_INSTALL_DIR folder for installing translations.

include(GNUInstallDirs)

set(APP_BIN_INSTALL_DIR    ${CMAKE_INSTALL_BINDIR})
set(APP_DEF_INSTALL_DIR    ${CMAKE_INSTALL_DATADIR}/applications)
set(APP_ICONS_INSTALL_DIR  ${CMAKE_INSTALL_DATADIR}/icons)
set(APP_LOCALE_INSTALL_DIR ${CMAKE_INSTALL_LOCALEDIR})

# From https://cmake.org/cmake/help/latest/command/install.html
#
# As absolute paths are not supported by cpack installer generators, it is
# preferable to use relative paths throughout. In particular, there is no need
# to make paths absolute by prepending CMAKE_INSTALL_PREFIX; this prefix is
# used by default if the DESTINATION is a relative path.
#
# But in FindKDE4Internal.cmake for unix uses absolute paths.
if (${FRAMEWORK} MATCHES KDE)
    string(REPLACE "${CMAKE_INSTALL_PREFIX}/" "" APP_BIN_INSTALL_DIR    ${BIN_INSTALL_DIR})
    string(REPLACE "${CMAKE_INSTALL_PREFIX}/" "" APP_DEF_INSTALL_DIR    ${XDG_APPS_INSTALL_DIR})
    string(REPLACE "${CMAKE_INSTALL_PREFIX}/" "" APP_ICONS_INSTALL_DIR  ${ICON_INSTALL_DIR})
    string(REPLACE "${CMAKE_INSTALL_PREFIX}/" "" APP_LOCALE_INSTALL_DIR ${LOCALE_INSTALL_DIR})
endif ()
