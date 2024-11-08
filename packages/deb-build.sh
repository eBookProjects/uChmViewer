#!/usr/bin/env bash

set -e

#MODE_NO_CLEAN=1
#MODE_QUIET=1

PACKAGE_ARCH=$(dpkg --print-architecture)
PACKAGE_INSTALL_PREFIX=/usr

SCRIPT_DIR=$(realpath "$(dirname "$0")")
SOURCE_DIR=$(realpath "$SCRIPT_DIR"/../)

clean()
{
    if [ -f "${PACKAGE_FILE_NAME}.deb" ]; then
        echo "Remove the old package ${PACKAGE_FILE_NAME}.deb"
        rm "${PACKAGE_FILE_NAME}.deb"
    fi

    if [ -d  "${PACKAGE_DIR}/DEBIAN" ]; then
		echo "Remove ${PACKAGE_DIR}/DEBIAN"
		rm -r "${PACKAGE_DIR}/DEBIAN"
    fi
}

pre_build()
{
    QMAKE_OPTIONS+=" PREFIX=$PACKAGE_INSTALL_PREFIX"
    $QMAKE -r $QMAKE_OPTIONS ${SOURCE_DIR}
}

build() {
    make -j $MAKE_JOBS
}

i18n()
{
    make -j $MAKE_JOBS i18n
}

deploy()
{
    make INSTALL_ROOT="$PACKAGE_DIR" install
}

package()
{
    mkdir -p "$PACKAGE_DIR"/DEBIAN

    echo -n "Getting depends..."
    for bin_obj in "$(find "$PACKAGE_BIN_DIR" -type f -name "*")"; do
        BINS+=("${bin_obj}")
    done
    PACKAGE_DEPENDS=$(get_deb_dependies "$PACKAGE_ARCH" ${BINS[@]})
    echo " Done"

    # https://www.debian.org/doc/debian-policy/ch-controlfields.html
    cat <<EOF > "$PACKAGE_DIR"/DEBIAN/control
Package: ${PACKAGE_NAME}-${PACKAGE_BROWSER}
Provides: $PACKAGE_NAME
Replaces: $PACKAGE_NAME
Breaks: $PACKAGE_NAME
Version: $PACKAGE_VERSION
Description: CHM and EBOOK viewer.
Section: devel
Priority: optional
Architecture: $PACKAGE_ARCH
Depends: $PACKAGE_DEPENDS
Maintainer: Nick Egorrov <nicegorov@yandex.ru>
Installed-Size: $(du -s "$PACKAGE_DIR" | awk '{print $1}')
EOF

    dpkg-deb -v --build "$PACKAGE_DIR" "${PACKAGE_FILE_NAME}.deb"
}

source ${SCRIPT_DIR}/build-helper.sh

create package
