
TEMPLATE = lib
TARGET = chm
CONFIG *= c++11 warn_on staticlib

HEADERS = \
    CHMLib/src/chm_lib.h \
    CHMLib/src/lzx.h

SOURCES = \
    CHMLib/src/chm_lib.c \
    CHMLib/src/lzx.c

DEFINES *= ffs=__builtin_ffs

win32:{
    DEFINES *= PPC_BSTR UNICODE
}
