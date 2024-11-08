
get_version()
{
    cat ${SOURCE_DIR}/src/version.h | grep 'define\s*APP_VERSION' | awk -F'"' '{print $(NF-1)}'
}

PACKAGE_NAME=uchmviewer
PACKAGE_VERSION=$(get_version)
PACKAGE_BROWSER=webkit
BUILD_DIR=${SOURCE_DIR}/build

usage()
{
  echo  -e "\
Build the application and create the package.\n\
Usage:\n\
$(basename "$0") [-b bld_dir] [-p pkg_dir] [-q qmake] [-s suffix] [-v version] [-w browser]\n\
\n\
Options:\n\
  -b bld_dir  The directory where the application will be built.\n\
              By default it is <source_root>/build\n\
  -p pkg_dir  The directory where the package will be built.\n\
              By default this is <bld_dir>/<pkg_name>_<pkg_version>\n\
  -q qmake    The name or full path of the qmake utility. In some\n\
              distributives, qmake for Qt 6 is named qmake6.\n\
  -s suffux   The suffix that will be added to the package name after the\n\
              version and before the architecture type.\n\
              Nothing is added by default.\n\
  -v version  Define the package version. By default version gettings from\n\
              the version.h file.\n\
  -w browser  Define the webengine to be used. Possible values:\n\
              WebEngine or WebKit (default).\n\
\n\
It is assumed that the necessary dependencies and tools are already installed\n\
in the system.\n\
\n\
The created package is saved in the current working directory.\
" 1>&2
}

MAKE_JOBS=$(nproc)
QMAKE=qmake

while getopts ":b:p:q:s:v:w:h" options; do
    case "${options}" in
    b)
        BUILD_DIR=${OPTARG}
        ;;
    p)
        PACKAGE_DIR=${OPTARG}
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
            echo -e "Error: unknown browser '${OPTARG}'.\n"
            usage
            exit 1
            ;;
        esac
        ;;
    h)
        usage
        exit 0
        ;;
    *)
        echo -e "Error: unknown option '-${OPTARG}'.\n"
        usage
        exit 1
        ;;
    esac
done

if ! [ -v FORCE_PACKAGE_DIR ]; then
    PACKAGE_DIR=${BUILD_DIR}/${PACKAGE_NAME}_${PACKAGE_VERSION}
fi
PACKAGE_BIN_DIR=${PACKAGE_DIR}/${PACKAGE_INSTALL_PREFIX}/bin
PACKAGE_FILE_NAME="${PACKAGE_NAME}-${PACKAGE_BROWSER}-${PACKAGE_VERSION}${PACKAGE_SUFFIX}_${PACKAGE_ARCH}"

echo_step()
{
    [ -v MODE_QUIET ] && return
    echo -e "\n================================================================================"
    echo -e "$1"
    echo -e "================================================================================\n"
}

# Returns 0 if there is no 'key' value in the 'list' array.
# param:
#   key    - key
#   list[] - list of a keys
is_uniquue()
{
    local item

    for item in "${@:2}"; do
        [[ ${item} == "${1}" ]] && return 1
    done

    return 0
}

# Returns a list of libraries on which the binaries depend
# param:
#   bins[] - list of a binaries
get_libs()
{
    local app
    local lib
    local lib_list

    for app in "${@}"; do
        for lib in $(objdump -p "${app}" | grep NEEDED | awk '{print $2}'); do
            if is_uniquue "${lib}" "${lib_list[@]}"; then
                lib_list+=("${lib}")
            fi
        done
    done

    echo "${lib_list[@]}"
}

# Returns a list of packages on which the binaries depend
# param:
#   arch   - architecture name
#   bins[] - list of a binaries
get_deb_packages()
{
    local pack
    local pack_list

    for pack in $(dpkg -S $(get_libs "${@:2}") | grep "${1}" | awk -F: '{print $1}'); do
        if is_uniquue "${pack}" "${pack_list[@]}"; then
            pack_list+=("${pack}")
        fi
    done

    echo "${pack_list[@]}"
}

# Returns a list of packages on which the binaries depend
# param:
#   arch   - architecture name
#   bins[] - list of a binaries
get_deb_dependies()
{
    local pack
    local dep_list
    local comma=0

    for pack in $(get_deb_packages "${1}" "${@:2}"); do
        ver=$(dpkg-query -f='${Version}' -W "${pack}:${1}")
        [[ ${comma} -eq 1 ]] && dep_list+=", "
        dep_list+="${pack} (>= ${ver})"
        comma=1
    done

    echo "${dep_list}"
}

create()
{
    CREATE_PACKAGE=0
    CREATE_PORTABLE=0

    for inst in "${@}"; do
        if [ ${inst} == package ]; then CREATE_PACKAGE=1; fi
        if [ ${inst} == portable ]; then CREATE_PORTABLE=1; fi
    done

    echo_step "\
Try to build package with:\n\
PACKAGE_NAME      ${PACKAGE_NAME}\n\
PACKAGE_BROWSER   ${PACKAGE_BROWSER}\n\
PACKAGE_SUFFIX    ${PACKAGE_SUFFIX}\n\
PACKAGE_VERSION   ${PACKAGE_VERSION}\n\
PACKAGE_ARCH      ${PACKAGE_ARCH}\n\
PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME}\n\
SOURCE_DIR        ${SOURCE_DIR}\n\
BUILD_DIR         ${BUILD_DIR}\n\
PACKAGE_DIR       ${PACKAGE_DIR}\n"

    if ! [ -v MODE_NO_CLEAN ]; then
        echo_step "Clean the build tree and the package dir"
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

    QMAKE_OPTIONS="USE_GETTEXT=1"

    if [ -v FORCE_VERSION ]; then
        QMAKE_OPTIONS+=" VERSION=$PACKAGE_VERSION"
    fi

    if [ ${PACKAGE_BROWSER} == webengine ]; then
        QMAKE_OPTIONS+=" USE_WEBENGINE=1"
    fi

    echo_step "Prepare to build"
    OLD_PWD=$PWD
    mkdir -p "$BUILD_DIR"
    mkdir -p "$PACKAGE_DIR"
    cd "$BUILD_DIR"
    pre_build

    echo_step "Make translations"
    i18n

    echo_step "Make application"
    build

    echo_step "Copy application to the package directory"
    deploy
    cd "$OLD_PWD"

    if [ ${CREATE_PACKAGE} == 1 ]; then
        echo_step "Create package"
        package
    fi

    if [ ${CREATE_PORTABLE} == 1 ]; then
        echo_step "Create portable"
        portable
    fi
}
