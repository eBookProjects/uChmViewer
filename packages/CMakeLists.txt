
cmake_minimum_required(VERSION 3.0)

if(${FRAMEWORK} STREQUAL KDE4)
    install(FILES kchmviewer.desktop DESTINATION ${KDE4_XDG_APPS_INSTALL_DIR})
    kde4_install_icons(${KDE4_ICON_INSTALL_DIR})
    kde4_update_iconcache()
endif()
