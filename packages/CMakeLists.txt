
cmake_minimum_required(VERSION 3.5)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install(FILES freedesktop/uchmviewer.desktop DESTINATION ${APP_DEF_INSTALL_DIR})
    install(DIRECTORY freedesktop/icons/ DESTINATION ${APP_ICONS_INSTALL_DIR})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND USE_MACOS_BUNDLE)
    set(PLIST macos/Info.plist)
    set(PLIST_OUT ${CMAKE_CURRENT_BINARY_DIR}/${PLIST})
    configure_file(${PLIST}.in ${PLIST_OUT})
    install(FILES ${PLIST_OUT} DESTINATION ${APP_ROOT_INSTALL_DIR})
    install(FILES macos/uchmviewer.icns DESTINATION ${APP_ICONS_INSTALL_DIR})
elseif (&{CMAKE_SYSTEM_NAME} MATCHES "Windows")
    install(FILES uchmviewer.png DESTINATION ${APP_ICONS_INSTALL_DIR})
endif ()


#
# Deployment section
################################################################################

if (USE_DEPLOY_RUNTIME)
    install(FILES $<TARGET_FILE:libzip::zip> DESTINATION ${APP_BIN_INSTALL_DIR})
    
    if (NOT USE_STATIC_CHMLIB)
        install(FILES $<TARGET_FILE:chm> DESTINATION ${APP_BIN_INSTALL_DIR})
    endif ()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        # Copying runtime dependencies for Windows deployment
        # See https://doc.qt.io/qt-5/windows-deployment.html
        set(QT_DEPLOY_DIR ${CMAKE_CURRENT_BINARY_DIR}/qt-deploy)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

        add_custom_command(TARGET kchmviewer POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-quick-import
                --dir ${QT_DEPLOY_DIR}
                --plugindir ${QT_DEPLOY_DIR}/plugins  $<TARGET_FILE:uchmviewer>
            COMMENT "Copying runtime dependencies"
        )

        install(DIRECTORY ${QT_DEPLOY_DIR}/ DESTINATION ${APP_BIN_INSTALL_DIR})
    endif ()
endif ()


#
# CPack section
################################################################################

if (NOT CPACK_PACKAGE_VERSION)
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
endif ()

set(CPACK_PACKAGE_CONTACT nicegorov@yandex.ru)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CHM and EBOOK viewer.")
set(CPACK_PACKAGE_DESCRIPTION [[
uChmViewer is a free, open-source chm (MS HTML help file format) viewer,
which uses Qt toolkit. Its main advantage is the best support for non-English
languages. Unlike other viewers, uChmViewer in most cases is able to correctly
detect the chm file encoding and show it. It correctly shows the index and
table of context in Russian, Spanish, Romanian, Korean, Chinese and Arabic help
files, and with new search engine is able to search in any chm file no matter
what language it is written.
]])

#set(CPACK_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/uchmviewer.png)
#set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
#set(CPACK_RESOURCE_FILE_README)
#set(CPACK_RESOURCE_FILE_WELCOME)

# Package specific
include(cpack/deb.cmake)

include(CPack)
