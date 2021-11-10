# The location of the installation directories. Only relative paths may be used.
#
#       Common:
#
# APP_BIN_INSTALL_DIR    installation folder of the executable.
# APP_ICONS_INSTALL_DIR  folder for installing application icons.
# APP_LOCALE_INSTALL_DIR folder for installing translations.
#
#       System-specific:
#
# APP_DEF_INSTALL_DIR    installation folder for a desktop entry in GNU/Linux.
# APP_ROOT_INSTALL_DIR   macOS bundle folder for Info.plist.

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
elseif (${CMAKE_SYSTEM_NAME}  MATCHES "Windows")
    set(APP_BIN_INSTALL_DIR    bin)
    set(APP_LOCALE_INSTALL_DIR locale)
    set(APP_ICONS_INSTALL_DIR  icons)
elseif (${CMAKE_SYSTEM_NAME}  MATCHES "Darwin" AND USE_MACOS_BUNDLE)
    set(APP_ROOT_INSTALL_DIR   ${PROJECT_NAME}.app/Contents)
    set(APP_BIN_INSTALL_DIR    ${APP_ROOT_INSTALL_DIR}/MacOS)
    set(APP_DATA_ROOT_DIR      ${APP_ROOT_INSTALL_DIR}/Resources)
    set(APP_LOCALE_INSTALL_DIR ${APP_DATA_ROOT_DIR}/locale)
    set(APP_ICONS_INSTALL_DIR  ${APP_DATA_ROOT_DIR})

    # maybe use CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT?
    if(${CMAKE_INSTALL_PREFIX} MATCHES "/usr/.*")
        set(CMAKE_INSTALL_PREFIX "~/Applications" CACHE PATH "" FORCE)
    endif()
endif ()
