
cmake_minimum_required(VERSION 3.0)

if (${USE_STATIC_CHMLIB})
    set(CHMLIB_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/CHMLib/src)

    if (EXISTS ${CHMLIB_SRC_DIR}/chm_lib.h)
        add_library(chm STATIC
            ${CHMLIB_SRC_DIR}/chm_lib.c
            ${CHMLIB_SRC_DIR}/lzx.c)
        target_include_directories(chm PUBLIC ${CHMLIB_SRC_DIR})
        #The PPC_BSTR macro is required when building on Windows.
        #The ffs = __ builtin_ffs macro is required when using MSYS.
        target_compile_definitions(chm PRIVATE
            ffs=__builtin_ffs
            UNICODE
            $<$<PLATFORM_ID:Windows>:PPC_BSTR>)
    else ()
        message(SEND_ERROR "The lib/CHMLib submodule was not found."
         "Use 'git submodule update --init' for static linking with chmlib.")
    endif ()
else ()
    find_package(chmlib REQUIRED)
endif ()

add_subdirectory(libebook)
