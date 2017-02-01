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

#include <CUnit/CUnit.h>
#include <stdio.h>
#include "../src/darray.h"

void create_array()
{
    DArray_t array = darray_create(sizeof(int), NULL, NULL);
    CU_ASSERT(array != NULL);
    CU_ASSERT(darray_size(array) == 0);
    CU_ASSERT(darray_capacity(array) == 0);
    darray_destroy(array);
}

void create_capacity_array()
{
    const size_t cap = 100;
    DArray_t array = darray_create_capacity(sizeof(int), NULL, NULL, cap);
    CU_ASSERT(array != NULL);
    CU_ASSERT(darray_size(array) == 0);
    CU_ASSERT(darray_capacity(array) == cap);
    darray_destroy(array);
}

void append_to_array()
{
    DArray_t array = darray_create(sizeof(int), NULL, NULL);
    int i = 1;
    darray_append(array, &i);
    CU_ASSERT(darray_size(array) == 1);
    CU_ASSERT(darray_capacity(array) == 1);
    darray_append(array, &i);
    CU_ASSERT(darray_size(array) == 2);
    CU_ASSERT(darray_capacity(array) == 2);
    darray_append(array, &i);
    CU_ASSERT(darray_size(array) == 3);
    CU_ASSERT(darray_capacity(array) == 4);
    darray_destroy(array);
}


int add_array_suite()
{
    CU_pSuite suite = CU_add_suite("darray-test", NULL, NULL);
    if (!suite) {
        fprintf(stderr,
                "unable to create darray suite: %s\n",
                CU_get_error_msg()
               );
        return CU_get_error();
    }

    CU_pTest test = CU_add_test(suite, "create", create_array);
    if (!test) {
        fprintf(stderr,
                "unable to create darray suite: %s\n",
                CU_get_error_msg()
               );
        return CU_get_error();
    }

    test = CU_ADD_TEST(suite, create_capacity_array);
    if (!test) {
        fprintf(stderr,
                "unable to create darray suite: %s\n",
                CU_get_error_msg()
               );
        return CU_get_error();
    }
    
    test = CU_ADD_TEST(suite, append_to_array);
    if (!test) {
        fprintf(stderr,
                "unable to create darray suite: %s\n",
                CU_get_error_msg()
               );
        return CU_get_error();
    }

    return CU_get_error();
}
