# Getting the application version from the version.h file.
# The result is saved in the PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR,
# PROJECT_VERSION_PATCH and PROJECT_VERSION variables.
# CMAKE_MATCH_n request CMake 3.9

cmake_minimum_required(VERSION 3.9)

file(READ ${CMAKE_SOURCE_DIR}/src/version.h VERSION_H)

string(REGEX MATCH "#define[ \t]*APP_VERSION_MAJOR[ \t]*([0-9]*)" _ ${VERSION_H})
if (CMAKE_MATCH_COUNT)
    set(PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(PROJECT_VERSION ${CMAKE_MATCH_1})
else ()
    return()
endif ()

string(REGEX MATCH "#define[ \t]*APP_VERSION_MINOR[ \t]*([0-9]*)" _ ${VERSION_H})
if (CMAKE_MATCH_COUNT)
    set(PROJECT_VERSION_MINOR ${CMAKE_MATCH_1})
    string(APPEND PROJECT_VERSION .${CMAKE_MATCH_1})
else ()
    return()
endif ()

string(REGEX MATCH "#define[ \t]*APP_VERSION_PATCH[ \t]*([0-9]*)" _ ${VERSION_H})
if (CMAKE_MATCH_COUNT)
    set(PROJECT_VERSION_PATCH ${CMAKE_MATCH_1})
    string(APPEND PROJECT_VERSION .${CMAKE_MATCH_1})
endif ()
