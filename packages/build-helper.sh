#!/usr/bin/env sh

if ! [ "$__NESTED__" = 1 ]; then
    echo "WARNING! This is an helper script."
    echo "Do not run the \"$(basename "$0")\" script directly,"
    echo "instead run the \"build-deb.sh\" or \"build-win-msys2.sh\" script."
    exit 2
fi

# Package specific
########################################
PACKAGE_NAME=uchmviewer
PACKAGE_LANG_LIST=$(cat "${SOURCE_DIR}/po/languages.txt")
PACKAGE_QT_QM_LIST="qt_ qtbase_"
PACKAGE_VERSION=$(cat "${SOURCE_DIR}"/src/version.h | grep 'define\s*APP_VERSION' | awk -F'"' '{print $(NF-1)}')
PACKAGE_BROWSER=webkit

# Default settings
########################################
BUILD_DIR=${SOURCE_DIR}/build
QMAKE=qmake
MAKE_JOBS=$(nproc)

usage()
{
    echo "\

Usage:
$(basename "$0") [-b bld_dir] [-p pkg_dir] [-q qmake] [-s suffix] [-v version] [-w browser]

Options:
  -b bld_dir  The directory where the application will be build.
              By default it is <source_root>/build
  -p pkg_dir  The directory where the package will be build.
              By default this is <bld_dir>/${PACKAGE_NAME}_<pkg_version>
  -q qmake    The name or full path of the qmake utility. In some
              distributives, qmake for Qt 6 is named qmake6.
  -s suffux   The suffix that will be added to the package name after the
              version and before the architecture type.
              Nothing is added by default.
  -v version  Define the package version. By default version gettings from
              the version.h file.
  -w browser  Define the webengine to be used. Possible values:
              WebEngine or WebKit (default).

It is assumed that the necessary dependencies and tools are already installed
in the system.

The created package is saved in the current working directory.
" 1>&2
}

help()
{
    echo "\
Build an application and create a package." 1>&2
usage
}

while getopts ":b:p:q:s:v:w:h" options; do
    case "${options}" in
    b)
        BUILD_DIR="$(realpath "${OPTARG}")"
        ;;
    p)
        PACKAGE_DIR="$(realpath "${OPTARG}")"
        FORCE_PACKAGE_DIR=1
        ;;
    q)
        QMAKE=${OPTARG}
        ;;
    s)
        PACKAGE_SUFFIX=_${OPTARG}
        ;;
    v)
        PACKAGE_VERSION=${OPTARG}
        FORCE_VERSION=1
        ;;
    w)
        case "${OPTARG}" in
        webkit | WebKit | WEBKIT )
            PACKAGE_BROWSER=webkit
            ;;
        webengine | WebEngine | WEBENGINE )
            PACKAGE_BROWSER=webengine
            ;;
        *)
            echo "Error: unknown browser '${OPTARG}'."
            usage
            exit 1
            ;;
        esac
        ;;
    h)
        help
        exit 0
        ;;
    *)
        echo "Error: unknown option '-${OPTARG}'."
        usage
        exit 1
        ;;
    esac
done

QT_MAJOR=$($QMAKE -query QT_VERSION  | awk -F . '{print $1}')

if ! [ "$FORCE_PACKAGE_DIR" = 1 ]; then
    PACKAGE_DIR=${BUILD_DIR}/${PACKAGE_NAME}_${PACKAGE_VERSION}
fi
PACKAGE_BIN_DIR=${PACKAGE_DIR}/${PACKAGE_INSTALL_PREFIX}/bin
PACKAGE_FILE_NAME="${PACKAGE_NAME}-${PACKAGE_VERSION}${PACKAGE_SUFFIX}-${PACKAGE_ARCH}-qt${QT_MAJOR}-${PACKAGE_BROWSER}"

title1()
{
    echo
    echo "================================================================================"
    echo "$1"
    echo "================================================================================"
    echo
}

title2()
{
    echo
    echo "$1"
    echo "========================================="
}

title3()
{
    echo
    echo "$1"
    echo "-------------------------"
}

# Work files
########################################
BIN_LIST_FILE=${BUILD_DIR}/get_dependies-bin.txt
LDD_LIST_FILE=${BUILD_DIR}/get_dependies-ldd.txt
OBJ_LIST_FILE=${BUILD_DIR}/get_dependies-obj.txt
LIB_LIST_FILE=${BUILD_DIR}/get_dependies-lib.txt
PKG_LIST_FILE=${BUILD_DIR}/get_dependies-pkg.txt
DEP_LIST_FILE=${BUILD_DIR}/get_dependies-dep.txt

# Calculates the dependencies and stores them in the variable PACKAGE_DEPENDS.
#
# get_deb_dependencies BIN_DIR
#   BIN_DIR - Directory with binaries for which dependencies are calculated.
################################################################
get_deb_dependencies()
{
	_bin_dir=$1
    title2 "Getting dependencies"
    echo "  BIN_DIR $_bin_dir"

    title3 "List of binaries:"
    find "$_bin_dir" -type f -name "*" > "$BIN_LIST_FILE"
    cat "$BIN_LIST_FILE"

    title3 "List of libraries from the ldd utility:"
    cat "$BIN_LIST_FILE" | xargs ldd | awk '{if (NF == 4) print "*"$3}' > "$LDD_LIST_FILE"
    cat "$LDD_LIST_FILE"

    title3 "List of libraries from the objdump utility:"
    cat "$BIN_LIST_FILE" | xargs objdump -p | grep NEEDED | awk '{print $2}'> "$OBJ_LIST_FILE"
    cat "$OBJ_LIST_FILE"

    title3 "Filtered list of libraries:"
    grep -F --file="$OBJ_LIST_FILE"  "$LDD_LIST_FILE" | sort -u > "$LIB_LIST_FILE"
    cat "$LIB_LIST_FILE"

    title3 "List of packages:"
    cat "$LIB_LIST_FILE" | xargs dpkg-query -S | awk '{print substr( $1, 1, length($1)-1 )}' | sort -u > "$PKG_LIST_FILE"
    cat "$PKG_LIST_FILE"

    title3 "String with dependencies:"
    cat "$PKG_LIST_FILE" | xargs xargs dpkg-query -W -f='${Package} (>= ${Version}), '| awk '{print substr( $0, 1, length($0)-2 )}' > "$DEP_LIST_FILE"
    cat "$DEP_LIST_FILE"
    PACKAGE_DEPENDS=$(cat "$DEP_LIST_FILE")
}

# Copies a Qt plugins files to the specified directory
#
# deploy_qt_plugins SRC DST
#   SRC - directory with installed Qt translations files
#   DST - destination directory
################################################################
deploy_qt_plugins()
{
    _src=$1
    _dst=$2

    if [ -f "$_src"/platforms/qwindows.dll ]; then
        title2 "Coping Qt plugins"
        echo "  SRC $_src"
        echo "  DST $_dst"

        mkdir -p "$_dst"/bearer
        mkdir -p "$_dst"/imageformats
        mkdir -p "$_dst"/platforms
        mkdir -p "$_dst"/printsupport
        mkdir -p "$_dst"/styles
        cp -v "$_src"/bearer/qgenericbearer.dll "$_dst"/bearer
        cp -v "$_src"/imageformats/*.dll "$_dst"/imageformats
        cp -v "$_src"/platforms/qwindows.dll "$_dst"/platforms
        cp -v "$_src"/printsupport/windowsprintersupport.dll "$_dst"/printsupport
        cp -v "$_src"/styles/qwindowsvistastyle.dll "$_dst"/styles
    fi
}

# Copies a Qt translations files to the specified directory
#
# deploy_qt_translations SRC DST
#   SRC - directory with installed Qt translations files
#   DST - destination directory
################################################################
deploy_qt_translations()
{
    _src=$1
    _dst=$2
    title2 "Coping a Qt translations files"
    echo "  SRC $_src"
    echo "  DST $_dst"

    for _lang in $PACKAGE_LANG_LIST; do
        title3 "Language code <${_lang}>"
        for _name in $PACKAGE_QT_QM_LIST; do
            _file=${_name}${_lang}.qm
            if [ -f "${_src}/${_file}" ]; then
                echo "${_file}"
                cp "${_src}/${_file}" "$_dst";
            fi
        done
    done
}

# Copies the DLLs needed for the binaries located in the specified directory.
#
# deploy_dlls BIN_DIR
#   BIN_DIR - Directory with binaries for which DLLs will be copied
################################################################
deploy_dlls()
{
    _bin_dir=$1
    title2 "Deploy DLLs"
    echo "  BIN_DIR $_bin_dir"

    title3 "List of binaries:"
    find "$_bin_dir" -type f -name "*.exe" > "$BIN_LIST_FILE"
    cat "$BIN_LIST_FILE"

    title3 "List of DLLs:"
    ntldd -R $(cat "$BIN_LIST_FILE") | grep mingw | awk '{print $3}'| sort -u > "$LIB_LIST_FILE"
    cat "$LIB_LIST_FILE"

    cp $(cat "$LIB_LIST_FILE") "$_bin_dir"
}

create()
{
    CREATE_PACKAGE=0
    CREATE_PORTABLE=0

    for arg in "${@}"; do
        if [ "${arg}" = package ]; then CREATE_PACKAGE=1; fi
        if [ "${arg}" = portable ]; then CREATE_PORTABLE=1; fi
    done

    title1 "\
Try to build package with:

QT_MAJOR          ${QT_MAJOR}
PACKAGE_NAME      ${PACKAGE_NAME}
PACKAGE_BROWSER   ${PACKAGE_BROWSER}
PACKAGE_SUFFIX    ${PACKAGE_SUFFIX}
PACKAGE_VERSION   ${PACKAGE_VERSION}
PACKAGE_ARCH      ${PACKAGE_ARCH}
PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME}
SOURCE_DIR        ${SOURCE_DIR}
BUILD_DIR         ${BUILD_DIR}
PACKAGE_DIR       ${PACKAGE_DIR}
"
    if ! [ "$MODE_NO_CLEAN" = 1 ]; then
        title1 "Clean the build tree and the package dir"
        clean

        if [ -d  "$BUILD_DIR" ]; then
            echo "Remove $BUILD_DIR"
            rm -rf "$BUILD_DIR"
        fi

        if [ -d "$PACKAGE_DIR" ]; then
            echo "Remove $PACKAGE_DIR"
            rm -rf "$PACKAGE_DIR"
        fi
    fi

    QMAKE_OPTIONS="$QMAKE_OPTIONS USE_GETTEXT=1"

    if [ "$FORCE_VERSION" = 1 ]; then
        QMAKE_OPTIONS="$QMAKE_OPTIONS VERSION=$PACKAGE_VERSION"
    fi

    if [ "$PACKAGE_BROWSER" = webengine ]; then
        QMAKE_OPTIONS="$QMAKE_OPTIONS USE_WEBENGINE=1"
    fi

    title1 "Prepare to build"
    OLD_PWD=$PWD
    mkdir -p "$BUILD_DIR"
    mkdir -p "$PACKAGE_DIR"
    cd "$BUILD_DIR"
    pre_build

    title1 "Make translations"
    i18n

    title1 "Make application"
    build

    title1 "Copy application to the package directory"
    deploy
    cd "$OLD_PWD"

    if [ "${CREATE_PACKAGE}" = 1 ]; then
        title1 "Create package"
        package
    fi

    if [ "${CREATE_PORTABLE}" = 1 ]; then
        title1 "Create portable"
        portable
    fi
}
