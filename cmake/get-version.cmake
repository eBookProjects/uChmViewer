# Getting the application version from the version.h file.
# The result is saved in the APP_VERSION cache variables.
# CMAKE_MATCH_n request CMake 3.9

set (f_ver "src/version.h")

if (EXISTS "${CMAKE_SOURCE_DIR}/${f_ver}")
    file(READ "${CMAKE_SOURCE_DIR}/${f_ver}" VERSION_H)

    string(REGEX MATCH "#define[ \t]*APP_VERSION[ \t]*\"(.*)\"" _ ${VERSION_H})
    if (CMAKE_MATCH_COUNT)
        set(APP_VERSION ${CMAKE_MATCH_1} CACHE STRING "")
        message("-- Application version ${APP_VERSION}")
    else ()
        message(WARNING "In get version: cannot parse the application version")
    endif ()
else ()
    message(WARNING "In get version: the '${f_ver}' file does not exist")
endif()
