/*
This file is part of c-lib

c-lib is free software: you can redistribute it and/or modify
it under the terms of the Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

c-lib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
Lesser General Public License for more details.

You should have received a copy of the Lesser General Public License
along with c-lib.  If not, see <http://www.gnu.org/licenses/>
*/


#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <getopt.h>
#include "suites.h"

int g_verbose = 0;
int g_silent = 0;

const char* optstring = "sv";

/**
 * returns an error code of the CUnit framework
 */
int add_suites()
{
    int res;
    
    res = add_array_suite();
    if (res)
        return res;
    
    res = add_list_suite();
    if (res)
        return res;

    res = add_stack_suite();
    if (res)
        return res;

    return res;
}

int parse_opts(int argc, char** argv) {
    int c;
    while ((c = getopt(argc, argv, optstring)) > 0) {
        switch(c) {
            case 's': g_silent = 1; break;
            case 'v': g_verbose = 1; break;
            default:
                fprintf(stderr, "unknown option encounterd.\n");
                exit(EXIT_FAILURE);
        }
    }

    if (g_verbose && g_silent) //cancel them.
        g_verbose = g_silent = 0;

    return 0;
}

int main(int argc, char** argv) {
    
    if(CU_initialize_registry() != CUE_SUCCESS) {
        fprintf(stderr, "Unable to initialze the unittesting frame work\n");
        return EXIT_FAILURE;
    }

    add_suites();

    parse_opts(argc, argv);

    if(g_verbose)
        CU_basic_set_mode(CU_BRM_VERBOSE);
    if(g_silent)
        CU_basic_set_mode(CU_BRM_SILENT);

    if (CU_basic_run_tests() != CUE_SUCCESS) {
        fprintf(stderr, "unable to run tests.\n");
        return EXIT_FAILURE;
    }

    CU_cleanup_registry();
}
