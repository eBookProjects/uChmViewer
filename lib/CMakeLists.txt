
cmake_minimum_required(VERSION 3.0)

set(_CHMLIB_ROOT ${CMAKE_CURRENT_LIST_DIR}/CHMLib)
set(_CHMLIB_SRC ${_CHMLIB_ROOT}/src)

if (EXISTS ${_CHMLIB_SRC}/chm_lib.h)
    set(CPP_SOURCES
        ${_CHMLIB_SRC}/chm_lib.c
        ${_CHMLIB_SRC}/lzx.c
        )

    add_library(chm STATIC ${CPP_SOURCES})
    target_include_directories(chm PUBLIC ${_CHMLIB_SRC})

    set_target_properties(chm PROPERTIES POSITION_INDEPENDENT_CODE ON)

    #The PPC_BSTR macro is required when building on Windows.
    #The ffs = __ builtin_ffs macro is required when using MSYS.
    target_compile_definitions(chm PRIVATE ffs=__builtin_ffs UNICODE $<$<PLATFORM_ID:Windows>:PPC_BSTR>)
else ()
    find_package(chmlib REQUIRED)
endif ()

unset(_CHMLIB_ROOT)
unset(_CHMLIB_SRC)

add_subdirectory(libebook)
