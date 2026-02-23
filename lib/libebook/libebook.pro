
TEMPLATE = lib
TARGET = ebook
CONFIG *= staticlib
QT += widgets xml

include(../../common.pri)

HEADERS += \
    bitfiddle.h \
    ebook_chm.h \
    ebook_epub.h \
    ebook.h \
    ebook_chm_encoding.h \
    ebook_epub_helperxml.h \
    ebook_search.h \
    helper_entitydecoder.h \
    helper_search_index.h \
    helperxmlhandler_epubcontainer.h \
    helperxmlhandler_epubcontent.h \
    helperxmlhandler_epubtoc.h

SOURCES += \
    ebook_chm.cpp \
    ebook_epub.cpp \
    ebook.cpp \
    ebook_chm_encoding.cpp \
    ebook_epub_helperxml.cpp \
    ebook_search.cpp \
    helper_entitydecoder.cpp \
    helper_search_index.cpp \
    helperxmlhandler_epubcontainer.cpp \
    helperxmlhandler_epubcontent.cpp \
    helperxmlhandler_epubtoc.cpp

!isEmpty(USE_STATIC_CHMLIB) {
    INCLUDEPATH *= ../CHMLib/src
} else {
    !isEmpty(CHMLIB_ROOT): INCLUDEPATH *= "$$CHMLIB_ROOT/include"
}

!isEmpty(LIBZIP_ROOT): INCLUDEPATH *= "$$LIBZIP_ROOT/include"

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
