

set (CLIB_SOURCES
    darray.c
    list.c
    )

set (CLIB_HEADERS
    darray.h
    list.h
    priv/listpriv.h
    )

add_library(${CLIB_SHARED_LIB} SHARED ${CLIB_SOURCES} ${CLIB_HEADERS})
add_library(${CLIB_STATIC_LIB} STATIC ${CLIB_SOURCES} ${CLIB_HEADERS})

#Make linking work for dynamic and shared libs
set_target_properties(${CLIB_SHARED_LIB} PROPERTIES
    COMPILE_FLAGS -DBUILD_CLIB_SHARED
    )
set_target_properties(${CLIB_STATIC_LIB} PROPERTIES
    COMPILE_FLAGS -DCLIB_STATIC_DEFINE
    )

#enable compiling with C99 standard
set_property(TARGET ${CLIB_SHARED_LIB} PROPERTY C_STANDARD 99)
set_property(TARGET ${CLIB_STATIC_LIB} PROPERTY C_STANDARD 99)

set_property(TARGET ${CLIB_SHARED_LIB} PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET ${CLIB_STATIC_LIB} PROPERTY C_STANDARD_REQUIRED ON)

#Make linking work for dynamic and shared libs
generate_export_header(${CLIB_STATIC_LIB} BASE_NAME clib)

