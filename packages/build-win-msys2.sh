#!/usr/bin/env sh

set -e

#MODE_NO_CLEAN=1

if [ "$MSYSTEM" = MINGW32 ]; then
    PACKAGE_ARCH=i686
else
    PACKAGE_ARCH=x86_64
fi

SCRIPT_DIR=$(realpath "$(dirname "$0")")
SOURCE_DIR=$(realpath "$SCRIPT_DIR"/../)

__NESTED__=1
. "${SCRIPT_DIR}"/build-helper.sh

clean()
{
    if [ -f "${PACKAGE_FILE_NAME}".exe ]; then
        echo "Remove old installer ${PACKAGE_FILE_NAME}.exe"
        rm "${PACKAGE_FILE_NAME}".exe
    fi

    if [ -f "$PACKAGE_FILE_NAME".zip ]; then
        echo "Remove old portable ${PACKAGE_FILE_NAME}.zip"
        rm "${PACKAGE_FILE_NAME}".zip
    fi
}

pre_build()
{
    QMAKE_OPTIONS="$QMAKE_OPTIONS USE_STATIC_CHMLIB=1 PREFIX=$PACKAGE_DIR"
    echo "$QMAKE -r $QMAKE_OPTIONS ${SOURCE_DIR}"
    $QMAKE -r $QMAKE_OPTIONS "$SOURCE_DIR"
}

build()
{
    make -j $MAKE_JOBS
}

i18n()
{
    make -j $MAKE_JOBS i18n
}

deploy()
{
    PACKAGE_BIN_DIR=${PACKAGE_DIR}/bin
    PACKAGE_I18N_DIR=${PACKAGE_BIN_DIR}/translations
    QT_PLUGINS=$($QMAKE -query QT_INSTALL_PLUGINS)
    QT_I18N_DIR=$($QMAKE -query QT_INSTALL_TRANSLATIONS)
    mkdir -p "$PACKAGE_BIN_DIR"
    mkdir -p "$PACKAGE_I18N_DIR"
    make install
    cp "$SOURCE_DIR"/COPYING "$PACKAGE_DIR"/LICENSE.txt

cat <<EOF > "$PACKAGE_BIN_DIR"/qt.conf
; https://doc.qt.io/qt-5/qt-conf.html

[Paths]
Translations=translations
EOF

    deploy_qt_plugins "${QT_PLUGINS}" "$PACKAGE_BIN_DIR"
    deploy_qt_translations "$QT_I18N_DIR" "$PACKAGE_I18N_DIR"
    deploy_dlls "$PACKAGE_BIN_DIR"
}

package()
{
    if command -v makensis >/dev/null 2>&1; then
        cp "${SCRIPT_DIR}"/win-installer.nsi "${PACKAGE_DIR}"

        makensis \
          -DAPP_ARCH="$PACKAGE_ARCH" \
          -DINSTALLER_FILE="${OLD_PWD}/${PACKAGE_FILE_NAME}".exe \
          "${PACKAGE_DIR}"/win-installer.nsi
        rm "${PACKAGE_DIR}"/win-installer.nsi
    else
        echo "The makensis not found."
    fi
}

portable()
{
    if command -v zip >/dev/null 2>&1; then
        cd "${PACKAGE_DIR}/.."
        mkdir -p "${PACKAGE_BIN_DIR}/portable"
        zip -r "${OLD_PWD}/${PACKAGE_FILE_NAME}.zip" "$(basename "${PACKAGE_DIR}")"
        rm -rf "${PACKAGE_BIN_DIR}/portable"
        cd "$OLD_PWD"
    else
        echo "The zip not found."
    fi
}

create package portable
