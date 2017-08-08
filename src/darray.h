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

#ifndef DARRAY_H
#define DARRAY_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

typedef void* DArray_t;

typedef void (*da_free_func)(void* element);
typedef void* (*da_copy_func)(void* dest, const void* src, size_t n);

/**
 * inits a member of darray when resizing eg.
 *
 * @return 0 when successfull !0 when not.
 */
typedef int (*da_init_func)(void* element);

/**
 * create an empty array.
 *
 * creates an empty array capable to grow and shrink when neccesarry
 * 
 * @param element_size [in] the sizeof() an single element.
 * @param ff [in] the free func will be called when individual elements
 *                are erased from the array.
 * @param cf [in] the function used to copy an element into the array.
 *                if none is specified memcpy will be used.
 */
DArray_t darray_create(size_t element_size, da_free_func ff, da_copy_func cf);

/**
 * create an empty array.
 *
 * creates an empty array capable to grow and shrink when neccesarry.
 * the biggest difference it preallocates capacity for a number of elements
 * which might make it more efficient when you know in advance that you'll
 * need a lot of elements.
 * 
 * @param element_size [in] the sizeof() an single element.
 * @param ff [in] the free func will be called when individual elements
 *                are erased from the array.
 * @param cf [in] the function used to copy an element into the array.
 *                if none is specified memcpy will be used.
 */
DArray_t
darray_create_capacity(
        size_t       element_size,
        da_free_func ff,
        da_copy_func cf,
        size_t       capacity
        );

/**
 * Destroys an array
 *
 * If a free_func was specified on construction of the array it will
 * be called on all elements prior to destruction of the array itself.
 *
 * @param array [in] the array to be destroyed.
 */
void darray_destroy(DArray_t array);

/**
 * Returns the size of array.
 */
size_t darray_size(const DArray_t array);

/**
 * Get the number of items that can be stored without the need to resize
 * the allocated buffers.
 */
size_t darray_capacity(const DArray_t array);

/**
 * Get a pointer to an array element
 *
 * If you are storing pointers inside array, remember that the value
 * returned is a pointer to a pointer in that case.
 *
 * @param array[in] Pointer to the array.
 * @param n [in] The index to retrieve
 *
 * @param return a pointer to the element
 */
void* darray_get(DArray_t array, size_t n);

/**
 * Set an element in the array
 *
 * This function sets a new item inside of the array if the array
 * was created with a free_func specified it will be called on the item
 * which is in the place where the new item should be inserted.
 *
 * @param array with a number of items greater than N
 * @param n the location inside the array should be smaller than the size
 *        of the array.
 * @param item, the item to insert in the array.
 */
void darray_set(DArray_t array, size_t n, void* item);

/**
 * Append a item to the back of the array.
 *
 * @param item[in] 
 */
int darray_append(DArray_t array, void* item);

/**
 * Reserve extra size to the array to expand to.
 *
 * @param array[in] The array to expand.
 * @param capacity [in] new capacity should be larger then the current size.
 */
int darray_reserve_capacity(DArray_t array, size_t capacity);

/**
 * Resizes array to a new size
 *
 * @param array[in] the array to resize
 * @param size[in]  the new size of the array.
 * @param init_func[in] a function that gets a new pointer to initialize.
 *                      maybe NULL, then new elements won't be initialized.
 */
int darray_resize(DArray_t array, size_t size, da_init_func init_func);

/**
 * Insert new elements into the array
 *
 * @param array     [in,out] the array to insert to
 * @param src       [in] the pointer to read from
 * @param i         [in] the index at which to insert.
 * @param nelems    [in] the number of elements to insert.
 */
int darray_insert(DArray_t array, void* src, size_t i, size_t nelems);

#ifdef __cplusplus
}
#endif

#endif 
