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

#ifndef FUNCTION_TYPES_H
#define FUNCTION_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \brief A clib_free_func is used to deallocate memory.
 *
 * A clib_free_func is the counter part of an allocation func. Therefore,
 * if an element inside an container needs additional freeing
 */
typedef void  (*clib_free_func)(void* element);

/**
 * \brief A clib_copy_func is used to copy a number of bytes from source to
 * destination.
 *
 * Copy a number (n) of bytes from the source(src) location to a destination
 * (dest) location. This function matches memcpy signature, so you can
 * typically use memcpy/memmove where a copy function is expected. The return
 * function is expected to return a pointer to the destination.
 *
 * @param[out]  dest    A pointer to the destination location.
 * @param[in]   src     The location to copy from.
 * @param[in]   n       The number of bytes to copy.
 *
 * @return      A pointer to the destination location.
 */
typedef void* (*clib_copy_func)(void* dest, const void* src, size_t n);

/**
 * \brief A signature for a function that compares two objects.
 *
 * A compare function that compares two elements. It should be compatible 
 * for sorting algorithms for example, so make sure the elements have
 * a total order, using the specified ordering function.
 *
 * \return The function should return an integer less then zero when element1
 *         is smaller the element2. A value larger than zero when element 2
 *         is larger and 0, when the two elements are equal.
 */
typedef int   (*clib_compare_func)(void* element1, void* element2);

#ifdef __cplusplus
} //extern "C"{
#endif

#endif  /*FUNCTION_TYPES_H*/
