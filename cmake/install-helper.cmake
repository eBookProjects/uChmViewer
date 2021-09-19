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

if (${FRAMEWORK} STREQUAL KDE4)
    set(APP_BIN_INSTALL_DIR    ${KDE4_BIN_INSTALL_DIR})
    set(APP_DEF_INSTALL_DIR    ${KDE4_XDG_APPS_INSTALL_DIR})
    set(APP_ICONS_INSTALL_DIR  ${KDE4_ICON_INSTALL_DIR})
    set(APP_LOCALE_INSTALL_DIR ${LOCALE_INSTALL_DIR})
endif ()
