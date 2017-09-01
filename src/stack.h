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

#ifndef STACK_H
#define STACK_H

#include "function-types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* Stack_t;

enum StackResult {
    STACK_OK = 0,
    STACK_OUT_OF_MEM
};

/**
 * create an empty stack.
 *
 * @param element_size [in] the sizeof() a single element.
 * @param ff [in] the free func will be called when individual elements
 *                are popped from the stack. If NULL, free will be used.
 * @param cf [in] the function used to copy an element on the stack.
 *                if none is specified memcpy will be used.
 */
Stack_t
stack_create(size_t element_size, clib_free_func ff, clib_copy_func cf);

/**
 * Destroys the stack with all of its elements.
 */
void
stack_destroy(Stack_t stack);

/**
 * Pop the top element from the stack.
 *
 * Frees the head member of the stack, the size of the stack will
 * be decremented with 1.
 */
void
stack_pop(Stack_t stack);

/**
 * Acces the toplevel element of the stack.
 *
 * \return a pointer to the data, or NULL when empty.
 */
void*
stack_head(Stack_t stack);

/**
 * Push an element on the stack.
 *
 * If an element is succesfully pushed on the stack, the size 
 * is incremented with 1.
 *
 * returns 0 when succesfull.
 */
int
stack_push(Stack_t stack, const void* element);

/**
 * Get the number of items stored on the stack.
 */
size_t
stack_size(const Stack_t stack);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /*ifndef STACK_H */

