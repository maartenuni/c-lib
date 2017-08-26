

option (BUILD_UNIT_TEST
        "Whether or not to build the unit tests requires CUnit"
        ON
        )

if(BUILD_UNIT_TEST)
    find_library(LIB_CUNIT cunit)

    set(UNIT_TEST unit-test)
    set(UNIT_TEST_SOURCES
            unit_test.c
            array_tests.c
            list_tests.c
        )

    set(UNIT_TEST_HEADERS 
        suites.h
        )

    add_executable(${UNIT_TEST} ${UNIT_TEST_SOURCES} ${UNIT_TEST_HEADERS})
    target_link_libraries(${UNIT_TEST} ${CLIB_STATIC_LIB} ${LIB_CUNIT})
endif()

