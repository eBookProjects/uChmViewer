
TEMPLATE = subdirs
SUBDIRS = libebook

exists(CHMLib/src/chm_lib.h): {
    SUBDIRS += CHMLib
    CHMLib.file = CHMLib.pro
    libebook.depends = CHMLib
}
