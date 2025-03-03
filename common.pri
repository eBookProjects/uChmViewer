
CONFIG *= c++11

CONFIG(debug, debug|release) {
    CONFIG *= warn_on
} else {
    # Suppress debug message
    DEFINES *= QT_NO_DEBUG_OUTPUT QT_NO_INFO_OUTPUT
}

PROJECT_ROOT_DIR = $${PWD}

# If the msgfmt command is not found, reset the USE_GETTEX variable.
MSGFMT_MSG=$$system("msgfmt -V")
!contains(MSGFMT_MSG, "msgfmt"): unset(USE_GETTEXT)

!defined(PREFIX, var) {
    unix:PREFIX = /usr/local
    win32:PREFIX = $$(programfiles)
}

unix {
    APP_DATA_ROOT_DIR = share
    APP_BIN_DIR = bin
    APP_DEF_DIR = $${APP_DATA_ROOT_DIR}/applications
    APP_ICONS_DIR = $${APP_DATA_ROOT_DIR}/icons
    APP_LOCALE_DIR = $${APP_DATA_ROOT_DIR}/locale
}

win32 {
    APP_BIN_DIR = bin
    APP_ICONS_DIR = icons
    APP_LOCALE_DIR = locale
}


#
# function findFiles(dir, basename)
# Returns a list of files with this basename.
# The function searches for dir and subdir recursively.
#
defineReplace(findFiles) {
    dir = $$1
    base_name = $$2

    file_list = $$files($$dir/$$base_name)
    path_list = $$files($$dir/*)

    for(path, path_list) {
        file_list += $$findFiles($$path, $$base_name)
    }

    return ($$file_list)
}

defineReplace(getVersion) {
    version_file = src/version.h
    !exists ($${PROJECT_ROOT_DIR}/$${version_file}): error(In function getVersion: File \"$${version_file}\" not found)
    version_header = $$cat($${PROJECT_ROOT_DIR}/$${version_file}, lines)

    for (str, version_header) {
        sub_str=$$split(str, " ")

        contains (sub_str, APP_VERSION) {
        ver_str=$$find(sub_str, '".*"')
            !isEmpty(ver_str): return ($$split(ver_str, '"'))
        }
    }

    error(In function getVersion: APP_VERSION not found in $${version_file})
}


#------------------#
# iwyu support     #
#------------------#

# https://doc.qt.io/qt-5/qmake-manual.html
# https://wiki-qt-io-staging.herokuapp.com/Undocumented_QMake

defineReplace(iwyu_out) {
    return ($${OUT_PWD}/$$basename(1).log)
}

defined(CHECK_IWYU, var) {
    linux*:IWYU_FLAGS += -fPIE

    IWYU_FLAGS += $(DEFINES) -w $(INCPATH)

    for(include, IWYU_INCLUDES) {
        IWYU_FLAGS += -I$$include
    }

    IWYU_EXTRA_FLAGS += -Xiwyu --max_line_length=160 -Xiwyu --mapping_file=$${PROJECT_ROOT_DIR}/tools/iwyu.imp

    iwyu.output_function  = iwyu_out
    iwyu.commands = $${PROJECT_ROOT_DIR}/tools/iwyu_comp.sh $${IWYU_EXTRA_FLAGS} $${IWYU_FLAGS} ${QMAKE_FILE_NAME}
    iwyu.input = SOURCES
    iwyu.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += iwyu
}
