
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
