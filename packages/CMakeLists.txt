
cmake_minimum_required(VERSION 3.0)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    install(FILES freedesktop/kchmviewer.desktop DESTINATION ${APP_DEF_INSTALL_DIR})
    install(DIRECTORY freedesktop/icons/ DESTINATION ${APP_ICONS_INSTALL_DIR})
endif ()
