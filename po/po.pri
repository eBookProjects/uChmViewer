
QMAKE_EXTRA_TARGETS += i18n

defined(USE_GETTEXT, var): {
    I18N_IN = $${PWD}
    I18N_OUT_BASE = $$shadowed($${PWD})
    LANG_LIST = $$cat(languages.txt)

    for (LANG, LANG_LIST) {
        I18N_OUT = $${I18N_OUT_BASE}/$${LANG}/LC_MESSAGES
        mkpath($${I18N_OUT})
        write_file($${I18N_OUT}/uchmviewer.mo)
        translates.files += $${I18N_OUT}/uchmviewer.mo
        QMAKE_EXTRA_TARGETS += i18n_$${LANG}
        i18n.depends += i18n_$${LANG}
        i18n_$${LANG}.commands = msgfmt --check -f -o $${I18N_OUT}/uchmviewer.mo $${I18N_IN}/uchmviewer_$${LANG}.po
    }
    
    translates.base = $$I18N_OUT_BASE
    translates.path = $${PREFIX}/$${APP_LOCALE_DIR}

    INSTALLS += translates
}
