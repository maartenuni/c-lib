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
#include "../src/list.h"

/********************* utility functions ****************/

int int_cmp_func(const void* k1, const void* k2)
{
    const int *i1 = k1, *i2 = k2;
    return *i2 - *i1;
}

void print_int_list(List_t list)
{
    ListNode* n = list_begin(list);
    while (n) {
        int* data = n->data;
        if (n->next)
            fprintf(stdout, "%d, ", *data);
        else
            fprintf(stdout, "%d", *data);
        n = n->next;
    }
    fprintf(stdout, "\n");
}

/******************** tests for list *******************/

void create_list()
{
    List_t list = list_create(sizeof(int), NULL, NULL);
    CU_ASSERT(list != 0);
    CU_ASSERT(list_size(list) == 0);
    list_destoy(list);
}

void prepend_list() // also tests list_find a little.
{
    const int begin = 0, end = 10; 
    List_t list = list_create(sizeof(int), NULL, NULL);
    ListNode* n;
    for (int i = begin; i < end; i++)
        n = list_prepend(list, &i);

    CU_ASSERT(list_size(list) == end - begin);
    
    for (int i = end - 1; i >= begin; i--, n = n->next)
        CU_ASSERT(* ((int*) n->data) == i);

    for (int i = begin - 5; i < end + 5; i++) {
        n = list_find(list, &i, int_cmp_func);
        if (i >=begin && i < end) {
            CU_ASSERT(n != NULL);
        }
        else {
            CU_ASSERT(n == NULL);
        }
    }

    list_destoy(list);
}


void equality_list()
{
    List_t l1, l2;
    ListNode* n1 = NULL, *n2 = NULL;
    l1 = list_create(sizeof(int), NULL, NULL);
    l2 = list_create(sizeof(int), NULL, NULL);

    for (int i = 0; i < 10; i++) {
        n1 = list_insert(l1, NULL, &i);
        n2 = list_insert(l2, NULL, &i);
    }

    CU_ASSERT(list_compare(l1, l2, int_cmp_func) == 0);
}

void reverse_list()
{
    const int begin = 0, end = 10;
    List_t l1, l2;
    ListNode* n1 = NULL, *n2 = NULL;
    l1 = list_create(sizeof(int), NULL, NULL);
    l2 = list_create(sizeof(int), NULL, NULL);

    for (int i = begin, r = end - 1; i < end; i++, r--) {
        n1 = list_insert(l1, n1, &i);
        n2 = list_insert(l2, n2, &r);
    }
    CU_ASSERT(list_compare(l1, l2, int_cmp_func) != 0);
    list_reverse(l1);
    CU_ASSERT(list_compare(l1, l2, int_cmp_func) == 0);
}

void remove_list()
{
    List_t list = list_create(sizeof(int), NULL, NULL);
    ListNode* n;
    for (int i = 10; i > 0 ; i--)
        n = list_prepend(list, &i);

    print_int_list(list);
    CU_ASSERT(*(int*)n->data == 1);
    list_remove(list, n);
    n = list_begin(list);
    CU_ASSERT(*(int*)n->data == 2);

}

int add_list_suite()
{
    CU_pSuite suite = CU_add_suite("list-test", NULL, NULL);
    if (!suite) {
        fprintf(stderr,
                "unable to create list suite: %s\n",
                CU_get_error_msg()
               );
        return CU_get_error();
    }

    CU_pTest test = CU_add_test(suite, "create", create_list);
    if (!test) {
        fprintf(stderr,
                "unable to create list test: %s\n",
                CU_get_error_msg()
               );

        return CU_get_error();
    }
    
    test = CU_add_test(suite, "prepend", prepend_list);
    if (!test) {
        fprintf(stderr,
                "unable to create list test: %s\n",
                CU_get_error_msg()
               );

        return CU_get_error();
    }
    
    test = CU_add_test(suite, "equality", equality_list);
    if (!test) {
        fprintf(stderr,
                "unable to create list test: %s\n",
                CU_get_error_msg()
               );

        return CU_get_error();
    }
    test = CU_add_test(suite, "reverse", reverse_list);
    if (!test) {
        fprintf(stderr,
                "unable to create list test: %s\n",
                CU_get_error_msg()
               );

        return CU_get_error();
    }
    
    test = CU_add_test(suite, "remove", remove_list);
    if (!test) {
        fprintf(stderr,
                "unable to create list test: %s\n",
                CU_get_error_msg()
               );

        return CU_get_error();
    }

    return CU_get_error();
}
