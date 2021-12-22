# Tries to find chmlib

find_path(chmlib_INCLUDE_DIR NAMES chm_lib.h
    PATHS ${chmlib_ROOT}
    PATH_SUFFIXES chmlib/include include)

find_library(chmlib_LIBRARY NAMES chm
    PATHS ${chmlib_ROOT}
    PATH_SUFFIXES chmlib/lib lib)

if (chmlib_INCLUDE_DIR AND chmlib_LIBRARY)
   set(chmlib_FOUND TRUE)
endif ()


if (chmlib_FOUND)
   if (NOT chmlib_FIND_QUIETLY)
      message(STATUS "Found libchm: ${chmlib_LIBRARY}")
   endif (NOT chmlib_FIND_QUIETLY)

   add_library(libchm SHARED IMPORTED)
   set_target_properties(libchm PROPERTIES
         INTERFACE_INCLUDE_DIRECTORIES ${chmlib_INCLUDE_DIR}
         IMPORTED_LOCATION ${chmlib_LIBRARY}
         IMPORTED_IMPLIB ${chmlib_LIBRARY} # Windows
   )
else ()
   if (chmlib_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find libchm. Please install chmlib-devel package (may be also called libchm-devel)")
   endif ()
endif ()
