
cmake_minimum_required(VERSION 3.16)
# Need to include generated files.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project files
set(CPP_SOURCES
    browser-types.hpp
    config.cpp
    config.h
    dialog_chooseurlfromlist.cpp
    dialog_chooseurlfromlist.h
    dialog_setup.cpp
    dialog_setup.h
    i18n.cpp
    i18n.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mimehelper.cpp
    mimehelper.h
    navigationpanel.cpp
    navigationpanel.h
    recentfiles.cpp
    recentfiles.h
    settings.cpp
    settings.h
    tab_bookmarks.cpp
    tab_bookmarks.h
    tab_contents.cpp
    tab_contents.h
    tab_index.cpp
    tab_index.h
    tab_search.cpp
    tab_search.h
    toolbareditor.cpp
    toolbareditor.h
    toolbarmanager.cpp
    toolbarmanager.h
    textencodings.cpp
    textencodings.h
    treeitem_index.cpp
    treeitem_index.h
    treeitem_toc.cpp
    treeitem_toc.h
    viewwindowmgr.cpp
    )

set(MOC_HEADERS
    dialog_chooseurlfromlist.h
    dialog_setup.h
    mainwindow.h
    recentfiles.h
    tab_bookmarks.h
    tab_contents.h
    tab_index.h
    tab_search.h
    viewwindowmgr.h
    navigationpanel.h
    toolbarmanager.h
    toolbareditor.h
    )

# UI files
set(UIS
    dialog_about.ui
    dialog_setup.ui
    dialog_topicselector.ui
    mainwindow.ui
    navigatorpanel.ui
    tab_bookmarks.ui
    tab_contents.ui
    tab_index.ui
    tab_search.ui
    toolbareditor.ui
    window_browser.ui
    )

if (USE_WEBENGINE)
    list(APPEND CPP_SOURCES
        qtwebengine/viewwindow.cpp
        qtwebengine/viewwindow.h
        qtwebengine/dataprovider.cpp
        qtwebengine/dataprovider.h
        )
    list(APPEND MOC_HEADERS qtwebengine/viewwindow.h  qtwebengine/webenginepage.h)
else ()
    list(APPEND CPP_SOURCES
        qtwebkit/viewwindow.cpp
        qtwebkit/viewwindow.h
        qtwebkit/dataprovider.cpp
        qtwebkit/dataprovider.h
        )
    list(APPEND MOC_HEADERS qtwebkit/viewwindow.h)
endif ()

if (USE_DBUS)
    list(APPEND CPP_SOURCES dbus_interface.cpp dbus_interface.h)
    list(APPEND MOC_HEADERS dbus_interface.h)
endif ()

# Mac OS specific
if (USE_MAC_APP)
    list(APPEND CPP_SOURCES uchmviewerapp.cpp uchmviewerapp.h)
    list(APPEND MOC_HEADERS uchmviewerapp.h)
endif ()

qt_add_resources(RSRCS resources/images.qrc)

qt_wrap_ui(UI_SOURCES ${UIS})
qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
add_executable(uchmviewer WIN32 ${CPP_SOURCES} ${RSRCS} ${UI_SOURCES} ${MOC_SOURCES})
target_link_libraries(uchmviewer
    ebook
    chmlib
    extra
    Qt::Core
    Qt::Network
    Qt::PrintSupport
    Qt::Widgets
    Qt::Xml
    )

target_link_libraries(uchmviewer i18n)
# For Gettext to work correctly, we tell the application the location
# of the folders with binaries and translations.
target_compile_definitions(uchmviewer PRIVATE
        APP_INSTALL_DIR_BIN="${APP_BIN_INSTALL_DIR}"
        APP_INSTALL_DIR_LOCALE="${APP_LOCALE_INSTALL_DIR}")

install(TARGETS uchmviewer DESTINATION ${APP_BIN_INSTALL_DIR})
