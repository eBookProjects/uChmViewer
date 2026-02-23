
# Project files
set(CPP_SOURCES
    bitfiddle.h
    ebook.cpp
    ebook.h
    ebook_chm.cpp
    ebook_chm.h
    ebook_chm_encoding.cpp
    ebook_chm_encoding.h
    ebook_epub.cpp
    ebook_epub.h
    ebook_epub_helperxml.cpp
    ebook_epub_helperxml.h
    ebook_search.cpp
    ebook_search.h
    helper_entitydecoder.cpp
    helper_entitydecoder.h
    helper_search_index.cpp
    helper_search_index.h
    helperxmlhandler_epubcontainer.cpp
    helperxmlhandler_epubcontainer.h
    helperxmlhandler_epubcontent.cpp
    helperxmlhandler_epubcontent.h
    helperxmlhandler_epubtoc.cpp
    helperxmlhandler_epubtoc.h
    )

set(MOC_HEADERS
    ebook_search.h
    helper_search_index.h
    )

qt_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
add_library(ebook STATIC ${CPP_SOURCES} ${MOC_SOURCES})
target_link_libraries(ebook PRIVATE chmlib libzip::zip Qt::Xml Qt::Core Qt::Widgets)
target_include_directories(ebook PUBLIC  "./")

if (TARGET ${QT}::Core5Compat)
    target_link_libraries(ebook PRIVATE ${QT}::Core5Compat)
endif ()
