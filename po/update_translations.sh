#!/bin/sh
#
# Update pot and po files.
# Requires the GNU gettext toolkit and Qt's lupdate and lconvert tools.
#
# Initially, translations were only in KDE and GNU Gettext was used for this.
# This causes some difficulties when extracting strings for translation from
# *.ui files. Previously, an extractrc script was used which generated an rc.cpp
# file from *.ui files and then rc.cpp (and other cpp) was handled by xgettext.
# In this case, references to line locations in the ui file were obtained as
# translator comments. A similar result is obtained when converting *.ui files
# to *.h using uic.
#
# Now lupdate is used to extract strings for translation. The lconvert utility
# then makes a pot file from the ts file.
#
# Old way
#---------------------------------
# #. i18n: file: dialog_about.ui:14
# #. i18n: ectx: property (windowTitle), widget (QDialog, DialogAbout)
# #: mainwindow.cpp:970 ui_dialog_about.h:111 rc.cpp:3
# msgid "About kchmviewer"
# msgstr ""
#
# New way
#---------------------------------
# #: ../src/dialog_about.ui:14 ../src/mainwindow.cpp:1117
# msgid "About kchmviewer"
# msgstr ""

PWD_DIR=`pwd`
cd `dirname $0`

# The languages.txt file contains a list of locales.
read LANGUAGES < languages.txt
CAT_NAME=kchmviewer
POT_FILE=${CAT_NAME}.pot
TS_FILE=tmp.ts
POT_TMP=tmp.pot

echo Update pot and po files
echo Language list:
echo "  $LANGUAGES"

lupdate -tr-function-alias tr+=i18n,QT_TRANSLATE_NOOP+=ki18n ../src -ts ${TS_FILE}
lconvert -i ${TS_FILE} -o ${POT_TMP}
# Removing strings with "msgctxt", otherwise Qt Linguist doesn't show sources
# and forms correctly. Then merging the duplicates and next adding translations
# specifically for main.cpp.
# \n[\t ]*msgctxt[\S\t ]*$
sed -r '/msgctxt/d' ${POT_TMP} > ${POT_FILE}
msguniq -o ${POT_FILE} ${POT_FILE}
xgettext -C -c -j --from-code=utf-8 -o ${POT_FILE} -ki18n:1 -kki18n:1 ../src/main.cpp

# Update catalogs or create new.
for LANG in ${LANGUAGES}; do
	CAT="${CAT_NAME}_${LANG}.po"
	if [ -e  ${CAT} ]
	then
		echo Update ${CAT}
		msgmerge -v --lang=${LANG} --update ${CAT} ${POT_FILE} # -o ${CAT}
	else
		msginit --no-translator --locale=${LANG} -o ${CAT} -i ${POT_FILE}
	fi
done

rm ${TS_FILE}
rm ${POT_TMP}
cd ${PWD_DIR}
