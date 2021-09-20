
cmake_minimum_required(VERSION 3.0)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    install(FILES freedesktop/kchmviewer.desktop DESTINATION ${APP_DEF_INSTALL_DIR})
    install(DIRECTORY freedesktop/icons/ DESTINATION ${APP_ICONS_INSTALL_DIR})
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
Kchmviewer is a free, open-source chm (MS HTML help file format) viewer,
which uses Qt toolkit. Its main advantage is the best support for non-English
languages. Unlike other viewers, kchmviewer in most cases is able to correctly
detect the chm file encoding and show it. It correctly shows the index and
table of context in Russian, Spanish, Romanian, Korean, Chinese and Arabic help
files, and with new search engine is able to search in any chm file no matter
what language it is written.
]])
#set(CPACK_PACKAGE_INSTALL_DIRECTORY "kchmviewer")
set(CPACK_PACKAGE_ICON ./kchmviewer.png)
set(CPACK_RESOURCE_FILE_LICENSE ../COPYING)
#set(CPACK_RESOURCE_FILE_README)
#set(CPACK_RESOURCE_FILE_WELCOME)

# Package specific
include(cpack/deb.cmake)

include(CPack)
