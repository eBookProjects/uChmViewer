# uChmViewer build script with qmake
#
#    Options:
#
# LIBZIP_ROOT - the root directory in which the zip library is installed
# CHMLIB_ROOT - the root directory in which the chm library is installed
# USE_STATIC_CHMLIB - if defined, static linkage to the chm library is used
# USE_WEBENGINE - if defined, WebEngine is used, otherwise WebKit is used
#
###############################################################################

TEMPLATE = subdirs
SUBDIRS = lib src
src.depends = lib
