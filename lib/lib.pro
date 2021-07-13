
TEMPLATE = subdirs
SUBDIRS = libebook

!isEmpty(USE_STATIC_CHMLIB) {
    exists(CHMLib/src/chm_lib.h) {
        SUBDIRS += CHMLib
        CHMLib.file = CHMLib.pro
        libebook.depends = CHMLib
    } else {
        error(The lib/CHMLib submodule was not found. Use 'git submodule update --init' for static linking with chmlib.)
    }
}
