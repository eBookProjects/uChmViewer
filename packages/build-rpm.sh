#!/usr/bin/env sh

set -e

#MODE_NO_CLEAN=1

PACKAGE_ARCH=$(uname -m)
PACKAGE_INSTALL_PREFIX=/usr
SCRIPT_DIR=$(realpath "$(dirname "$0")")
SOURCE_DIR=$(realpath "$SCRIPT_DIR"/../)

__NESTED__=1
. ${SCRIPT_DIR}/build-helper.sh

clean()
{
    if [ -f "${PACKAGE_FILE_NAME}.rpm" ]; then
        echo "Remove the old package ${PACKAGE_FILE_NAME}.rpm"
        rm "${PACKAGE_FILE_NAME}.rpm"
    fi

    if [ -d  "$BUILD_DIR/rpmbuild" ]; then
        echo "Remove $BUILD_DIR/rpmbuild"
        rm -r "$BUILD_DIR/rpmbuild"
    fi
}

pre_build()
{
    QMAKE_OPTIONS="$QMAKE_OPTIONS PREFIX=$PACKAGE_INSTALL_PREFIX"
    echo "$QMAKE -r $QMAKE_OPTIONS ${SOURCE_DIR}"
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
	# https://docs.fedoraproject.org/en-US/packaging-guidelines/RPMMacros
    cat <<EOF > "$BUILD_DIR/$PACKAGE_NAME.spec"
Summary:   CHM and EBOOK viewer
Name:      uchmviewer
Version:   ${PACKAGE_VERSION}
Release:   0
License:   GPL-3.0-or-later
Group:     Applications/Office
Packager:  nicegorov@yandex.ru
URL:       https://github.com/eBookProjects/uChmViewer

%description
This is a fork of the kchmviewer with some improvements. KchmViewer is a
chm (MS HTML help file format) and epub viewer, written in C++.

%files
${PACKAGE_INSTALL_PREFIX}/bin/uchmviewer
${PACKAGE_INSTALL_PREFIX}/share/applications/uchmviewer.desktop
${PACKAGE_INSTALL_PREFIX}/share/doc/uchmviewer/AUTHORS.md
${PACKAGE_INSTALL_PREFIX}/share/doc/uchmviewer/ChangeLog
${PACKAGE_INSTALL_PREFIX}/share/doc/uchmviewer/NEWS.md
${PACKAGE_INSTALL_PREFIX}/share/doc/uchmviewer/README.md
${PACKAGE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps/uchmviewer.png
${PACKAGE_INSTALL_PREFIX}/share/locale/cs/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/de/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/fr/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/hu/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/it/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/nl/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/pl/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/pt_BR/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/ru/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/sv/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/tr/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/uk/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/zh_CN/LC_MESSAGES/uchmviewer.mo
${PACKAGE_INSTALL_PREFIX}/share/locale/zh_TW/LC_MESSAGES/uchmviewer.mo

%changelog

EOF

    title2 "rpmbuild"
    echo "rpmbuild --bb --buildroot "$PACKAGE_DIR" --define "_topdir $BUILD_DIR/rpmbuild" "$BUILD_DIR/$PACKAGE_NAME.spec""
    rpmbuild --bb --buildroot "$PACKAGE_DIR" --define "_topdir $BUILD_DIR/rpmbuild" "$BUILD_DIR/$PACKAGE_NAME.spec"
    mv "$BUILD_DIR/rpmbuild/RPMS/$PACKAGE_ARCH/"*.rpm $PWD/${PACKAGE_FILE_NAME}.rpm
}

create package
