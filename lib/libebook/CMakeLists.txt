
cmake_minimum_required(VERSION 3.5)

# Project files
set(CPP_SOURCES
    ebook_chm.cpp
    ebook_epub.cpp
    ebook.cpp
    ebook_chm_encoding.cpp
    ebook_search.cpp
    helper_entitydecoder.cpp
    helper_search_index.cpp
    helperxmlhandler_epubcontainer.cpp
    helperxmlhandler_epubcontent.cpp
    helperxmlhandler_epubtoc.cpp
    )

set(MOC_HEADERS
    ebook_search.h
    helper_search_index.h
    )

qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
add_library(ebook STATIC ${CPP_SOURCES} ${MOC_SOURCES})
target_link_libraries(ebook PRIVATE chm libzip::zip Qt::Xml Qt::Core Qt::Widgets)
target_include_directories(ebook PUBLIC  "./")

if (TARGET ${QT}::Core5Compat)
    target_link_libraries(ebook PRIVATE ${QT}::Core5Compat)
endif ()
