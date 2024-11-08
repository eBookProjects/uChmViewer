#!/usr/bin/env bash

set -e

#MODE_NO_CLEAN=1
#MODE_QUIET=1

if [ "$MSYSTEM" == MINGW32 ]; then
    PACKAGE_ARCH=i686
else
    PACKAGE_ARCH=x86_64
fi

SCRIPT_DIR=$(realpath "$(dirname "$0")")
SOURCE_DIR=$(realpath "$SCRIPT_DIR"/../)

clean()
{
    if [ -f "${PACKAGE_FILE_NAME}".exe ]; then
        echo "Remove old installer ${PACKAGE_FILE_NAME}.exe"
        rm "${PACKAGE_FILE_NAME}".exe
    fi

    if [ -f "$PACKAGE_FILE_NAME".zip ]; then
        echo "Remove old portable ${PACKAGE_FILE_NAME}.zip"
        rm ${PACKAGE_FILE_NAME}.zip
    fi
}

pre_build()
{
    QMAKE_OPTIONS+=" USE_STATIC_CHMLIB=1 PREFIX=${PACKAGE_DIR}"
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
    PACKAGE_BIN_DIR=${PACKAGE_DIR}/bin
    PACKAGE_I18N_DIR=${PACKAGE_BIN_DIR}/translations
    QT_PLUGINS=$($QMAKE -query QT_INSTALL_PLUGINS)
    QT_I18NS=$($QMAKE -query QT_INSTALL_TRANSLATIONS)
    mkdir -p "$PACKAGE_BIN_DIR"
    mkdir -p "$PACKAGE_I18N_DIR"
    make install
    cp "$SOURCE_DIR"/COPYING "$PACKAGE_DIR"/LICENSE.txt

cat <<EOF > "$PACKAGE_BIN_DIR"/qt.conf
; https://doc.qt.io/qt-5/qt-conf.html

[Paths]
Translations=translations
EOF

    if [ -f "${QT_PLUGINS}"/platforms/qwindows.dll ]; then
        echo "Coping Qt plugins"
        mkdir -p "$PACKAGE_BIN_DIR"/bearer
        mkdir -p "$PACKAGE_BIN_DIR"/imageformats
        mkdir -p "$PACKAGE_BIN_DIR"/platforms
        mkdir -p "$PACKAGE_BIN_DIR"/printsupport
        mkdir -p "$PACKAGE_BIN_DIR"/styles
        cp "${QT_PLUGINS}"/bearer/qgenericbearer.dll "$PACKAGE_BIN_DIR"/bearer
        cp "${QT_PLUGINS}"/imageformats/*.dll "$PACKAGE_BIN_DIR"/imageformats
        cp "${QT_PLUGINS}"/platforms/qwindows.dll "$PACKAGE_BIN_DIR"/platforms
        cp "${QT_PLUGINS}"/printsupport/windowsprintersupport.dll "$PACKAGE_BIN_DIR"/printsupport
        cp "${QT_PLUGINS}"/styles/qwindowsvistastyle.dll "$PACKAGE_BIN_DIR"/styles
    fi

    echo "Coping Qt translations"
    for lang in 'cs' 'de' 'fr' 'hu' 'it' 'nl' 'pt_BR' 'ru' 'sv' 'tr' 'uk' 'zh_CN' 'zh_TW'; do
        [ -f "${QT_I18NS}/qt_${lang}.qm" ] && cp "${QT_I18NS}/qt_${lang}.qm" "$PACKAGE_I18N_DIR"
        [ -f "${QT_I18NS}/qtbase_${lang}.qm" ] && cp "${QT_I18NS}/qtbase_${lang}.qm" "$PACKAGE_I18N_DIR"
        [ -f "${QT_I18NS}/qtserialport_${lang}.qm" ] && cp "${QT_I18NS}/qtserialport_${lang}.qm" "$PACKAGE_I18N_DIR"
    done

    echo "Coping DLL's"
    for dll in $(ldd $(find "$PACKAGE_BIN_DIR" -type f -name "*.exe") | sort -u | grep mingw | awk '{print $1}'); do
        cp "$(which "$dll")" "$PACKAGE_BIN_DIR"
    done
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
        zip -r "${OLD_PWD}/${PACKAGE_FILE_NAME}.zip" "$(basename "${PACKAGE_DIR}")"
        cd "$OLD_PWD"
    else
        echo "The zip not found."
    fi
}

source ${SCRIPT_DIR}/build-helper.sh

create package portable
