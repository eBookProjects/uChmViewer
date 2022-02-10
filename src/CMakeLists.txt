
cmake_minimum_required(VERSION 3.0)
# Need to include generated files.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project files
set(CPP_SOURCES
    config.cpp
    dialog_chooseurlfromlist.cpp
    dialog_setup.cpp
    main.cpp
    mainwindow.cpp
    recentfiles.cpp
    settings.cpp
    tab_bookmarks.cpp
    tab_contents.cpp
    tab_index.cpp
    tab_search.cpp
    navigationpanel.cpp
    toolbarmanager.cpp
    toolbareditor.cpp
    textencodings.cpp
    treeitem_toc.cpp
    treeitem_index.cpp
    mimehelper.cpp
    i18n.cpp
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

if (${USE_WEBENGINE})
    list(APPEND CPP_SOURCES
        qtwebengine/viewwindow.cpp
        qtwebengine/dataprovider.cpp
        qtwebengine/viewwindowmgr.cpp
        )
    list(APPEND MOC_HEADERS qtwebengine/viewwindow.h  qtwebengine/webenginepage.h)
else ()
    list(APPEND CPP_SOURCES
        qtwebkit/viewwindow.cpp
        qtwebkit/dataprovider.cpp
        qtwebkit/viewwindowmgr.cpp
        )
    list(APPEND MOC_HEADERS qtwebkit/viewwindow.h)
endif ()

if (${USE_DBUS})
    list(APPEND CPP_SOURCES dbus_interface.cpp)
    list(APPEND MOC_HEADERS dbus_interface.h)
endif ()

# Mac OS specific
if (${USE_MAC_APP})
    list(APPEND CPP_SOURCES uchmviewerapp.cpp)
    list(APPEND MOC_HEADERS uchmviewerapp.h)
endif ()

qt_add_resources(RSRCS resources/images.qrc)

qt_wrap_ui(UI_SOURCES ${UIS})
qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
add_executable(uchmviewer ${CPP_SOURCES} ${RSRCS} ${UI_SOURCES} ${MOC_SOURCES})
target_link_libraries(uchmviewer
    ebook
    chm
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
