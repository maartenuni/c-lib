/*
 * This file is part of c-lib
 *
 * Copyright © 2017 Maarten Duijndam
 *
 * c-lib is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * c-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser General Public License
 * along with c-lib.  If not, see <http://www.gnu.org/licenses/>
 */

#include <CUnit/CUnit.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/stack.h"

/* * utilities * */
const char* values [] = {
    "Every,",
    "Beautiful",
    "Guitar",
    "Deserves",
    "Amps",
    "Everyday"
};

const size_t val_size = sizeof(values);

char* strdup(const char* s)
{
    size_t len = strlen(s);
    char* ret = malloc(len + 1);
    if (ret) {
        memcpy(ret, s, len);
        ret[len] = '\0';
    }
    return ret;
}

void* clib_str_cpy(void* dest, const void* src, size_t n)
{
    char* temp = strdup((const char*) src);
    memcpy(dest, &temp, n);
    return dest;
}

void clib_pointer_free(void* ptr)
{
    char** fp = ptr;
    free(*fp);
}

/* * Tests * */

void create_stack()
{
    Stack_t stack = stack_create(sizeof(char*), NULL, NULL);
    CU_ASSERT(stack != 0);
    CU_ASSERT(stack_size(stack) == 0);
    stack_destroy(stack);
}

void item_manipulations()
{
    Stack_t stack = stack_create(sizeof(char*), clib_pointer_free, clib_str_cpy);
    for(size_t i = 0; i < val_size; i++) {
        int ret = stack_push(stack, &values[i]);
        CU_ASSERT(ret == STACK_OK);
    }
    CU_ASSERT(stack_size(stack) == val_size);
    for (int i = (int)stack_size(stack) - 1; i >= 0; i--) {
        char* strval = stack_head(stack);
        CU_ASSERT(strcmp(strval, values[i]) == 0);
    }
    stack_destroy(stack);
}

/* * Tests  registration * */

int add_stack_suite()
{
    CU_pSuite suite = CU_add_suite("stack-test", NULL, NULL);
    if (!suite) {
        fprintf(stderr,
                "unable to create stack suite: %s\n",
                CU_get_error_msg()
               );
        return CU_get_error();
    }

    CU_pTest test = CU_add_test(suite, "create", create_stack);
    if (!test) {
        fprintf(stderr,
                "unable to create stack test: %s\n",
                CU_get_error_msg()
               );

        return CU_get_error();
    }
    
    test = CU_add_test(suite, "item_manipulations", item_manipulations);
    if (!test) {
        fprintf(stderr,
                "unable to create stack test: %s\n",
                CU_get_error_msg()
               );

        return CU_get_error();
    }

// item_manipulations covers this.
//    test = CU_add_test(suite, "equality", equality_stack);
//    if (!test) {
//        fprintf(stderr,
//                "unable to create stack test: %s\n",
//                CU_get_error_msg()
//               );
//
//        return CU_get_error();
//    }

    return CU_get_error();
}
