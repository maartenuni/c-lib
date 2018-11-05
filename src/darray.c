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

#include "darray.h"
#include <string.h>
#include <assert.h>

/**
 * \brief the private implementation of an array.
 *
 * \private
 */
struct DArray {
    size_t  esize;   ///< Element size;
    size_t  size;    ///< number of elements contained
    size_t  cap;     ///< capacity of the buffer (in number of elements).
    char*   elems;   ///< pointer to the elements.

    da_free_func ff; ///< function called when erasing element from the array
    da_copy_func cf; ///< This function is called when a new member is inserted.
};

typedef struct DArray DArray;

const size_t DARRAY_SIZE    = sizeof(DArray);
const size_t DARRAY_INC     = 2;


//static array


DArray_t
darray_create(size_t element_size, da_free_func ff, da_copy_func cf)
{
    DArray*  ret = calloc(1, DARRAY_SIZE);
    if (ret) {
        ret->esize  = element_size;
        ret->ff     = ff;
        if (cf)
            ret->cf = cf;
        else
            ret->cf = memcpy;
    }
    return ret;
}

DArray_t
darray_create_capacity(
        size_t       element_size,
        da_free_func ff,
        da_copy_func cf,
        size_t       capacity
        )
{
    DArray*  ret = darray_create(element_size, ff, cf);
    if (ret) {
        darray_reserve_capacity(ret, capacity);
    }
    return ret;
}

void
darray_destroy(DArray_t array)
{
    DArray* ar = array;
    if (ar->ff) {
        for (size_t i = 0; i < darray_size(ar); ++i)
            ar->ff(darray_get(ar, i));
    }
    free(ar->elems);
    free(ar);
}

size_t
darray_size(const DArray_t array)
{
    const DArray* ar = array;
    return ar->size;
}

size_t
darray_capacity(const DArray_t array)
{
    const DArray* ar = array;
    return ar->cap;
}

void* darray_get(DArray_t array, size_t n)
{
    DArray* ar = array;
    return ar->elems + n * ar->esize;
}

void darray_set(DArray_t array, size_t n, void* item)
{
    DArray* ar = array;
    void* element = darray_get(ar, n);
    if(ar->ff)
        ar->ff(element);
    ar->cf(element, item, ar->esize);
}

int
darray_append(DArray_t array, void* item)
{
    DArray* ar = array;
    if (darray_size(ar) == darray_capacity(ar)) {
        size_t new_cap;
        int ret;
        new_cap = darray_capacity(ar) ? darray_capacity(ar) * DARRAY_INC : 1;
        ret = darray_reserve_capacity(ar, new_cap);
        if (ret)
            return ret;
    }
    void* dest = darray_get(ar, ar->size++);
    ar->cf(dest, item, ar->esize);
    return 0;
}

int 
darray_pop_back(DArray_t array, void*item)
{
    DArray* ar = array;
    int res = 0;
    ar->cf(item, darray_get(ar, ar->size - 1), ar->esize);
    if (darray_size(ar) < darray_capacity(ar)/4)
        res = darray_reserve_capacity(ar, darray_capacity(ar)/2);
    
    if (res == 0)
        ar->size--;
    return res;
}

int
darray_reserve_capacity(DArray_t array, size_t capacity)
{
    DArray* ar = array;
    assert(capacity >= darray_size(array));

    void* newbytes = realloc(ar->elems, ar->esize * capacity);
    if (newbytes)
        ar->elems = newbytes;
    else
        return 1;
    ar->cap = capacity;
    return 0;
}

int
darray_resize(DArray_t array, size_t size, da_init_func init)
{
    DArray* ar = array;
    int res = 0;
    if (size > ar->cap) {
        res = darray_reserve_capacity(ar, size);
        if (res)
            return res;
    }
    if (init) { 
        for (size_t i = darray_size(ar); i < size; i++)
            if (init(darray_get(ar, i)))
                res = 1;
    }

    if (size < darray_size(ar)) {
        for (size_t i = darray_size(ar) - 1; i >= size; i--)
            ar->ff(darray_get(ar, i));
    }

    if (size < darray_capacity(ar)/4)
        darray_reserve_capacity(ar, size);

    ar->size = size;
    return 0;
}

int
darray_insert(DArray_t array, void* src, size_t i, size_t nelems)
{
    int ret = 0;
    void* res;
    DArray* ar = array;

    size_t new_size = darray_size(ar) + nelems; // size when function finishes
    size_t diff = new_size - darray_size(ar);   // 
    size_t n_to_move = darray_size(ar) - i - 1; // n elements to move

    if (new_size >= darray_capacity(ar))
        ret = darray_reserve_capacity(ar, new_size);
    
    if (!ret)
        return ret;

    res = memmove(
            darray_get(ar, i + diff),
            darray_get(ar, i),
            n_to_move * ar->esize
            );
    
    if(res)
        memcpy(darray_get(ar,i),
               src,
               nelems * ar->esize
               );

    ar->size = new_size;
    return 0;
}
