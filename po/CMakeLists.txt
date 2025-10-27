# Create and install translation files.

set(CATALOG_NAME uchmviewer)
# The languages.txt file contains a list of locales.
file(READ languages.txt LANGUAGES)
string(REGEX REPLACE "[\t ]+" ";" LANGUAGES ${LANGUAGES})

add_library(i18n INTERFACE)

if (USE_GETTEXT)
    find_program(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

    if (GETTEXT_MSGFMT_EXECUTABLE)
        add_custom_target(update_translations
            COMMAND sh update_translations.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    else ()
        set(USE_GETTEXT OFF)
        message(STATUS "USE_GETTEXT flag set, but the GNU gettext toolset was not found.")
    endif ()
endif ()

if (USE_GETTEXT)
    find_package(Intl)

    if (Intl_FOUND)
        target_compile_definitions(i18n INTERFACE USE_GETTEXT)
        target_include_directories(i18n INTERFACE ${Intl_INCLUDE_DIRS})
        target_link_libraries(i18n INTERFACE ${Intl_LIBRARIES})
    else ()
        set(USE_GETTEXT OFF)
        message(STATUS "USE_GETTEXT flag set, but the GNU libintl headers and libraries was not found.")
    endif ()
endif ()

if (USE_GETTEXT)
    foreach (LANG ${LANGUAGES})
        set(CATALOG ${CMAKE_CURRENT_SOURCE_DIR}/${CATALOG_NAME}_${LANG}.po)
        if(EXISTS ${CATALOG})
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LANG}/LC_MESSAGES)
            set(GMO_FILE ${CMAKE_CURRENT_BINARY_DIR}/${LANG}/LC_MESSAGES/${CATALOG_NAME}.mo)
            list(APPEND GMO_LIST ${GMO_FILE})
            file(RELATIVE_PATH GMO_FILE_D ${CMAKE_BINARY_DIR} ${GMO_FILE})
            add_custom_command(OUTPUT ${GMO_FILE}
                COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -f -o ${GMO_FILE} ${CATALOG}
                DEPENDS ${CATALOG}
                COMMENT "Generate ${GMO_FILE_D}")
            install(FILES ${GMO_FILE} DESTINATION ${APP_LOCALE_INSTALL_DIR}/${LANG}/LC_MESSAGES)
        else ()
            file(RELATIVE_PATH CATALOG ${CMAKE_SOURCE_DIR} ${CATALOG})
            message(WARNING "File ${CATALOG}.po not exist")
        endif ()
    endforeach ()

    add_custom_target(translations ALL DEPENDS ${GMO_LIST})
else (USE_GETTEXT)
# TODO generate Qt translation files (*.qm)
endif (USE_GETTEXT)
