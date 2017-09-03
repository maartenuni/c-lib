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

#include "priv/stackpriv.h"
#include "stack.h"
#include <assert.h>
#include <stdlib.h>

struct StackClass {
    size_t  element_sz;
    void  (*construct)(struct Stack*, size_t, clib_free_func, clib_copy_func);
    void  (*destruct) (struct Stack*);
    size_t(*size)(const struct Stack*);
    void  (*pop)(struct Stack*);
    void* (*head)(struct Stack*);
    int   (*push)(struct Stack*, const void* element);
};

typedef struct StackClass StackClass;

static void
_stack_construct(struct Stack* self,
                 size_t element_size,
                 clib_free_func ff,
                 clib_copy_func cf
                 )
{
    self->list = list_create(element_size, ff, cf);
}

static void
_stack_destruct(struct Stack* self)
{
    list_destroy(self->list);
    free(self);
}

static size_t
_stack_size(const struct Stack* self)
{
    return list_size(self->list);
}


static void
_stack_pop(struct Stack* self)
{
    list_remove(self->list, list_begin(self->list));
}

static void*
_stack_head(Stack* self)
{
    ListNode* node = list_begin(self->list);
    return node->data;
}

static int 
_stack_push(Stack* self, const void* element)
{
    if (list_prepend(self->list, element) != NULL)
        return STACK_OK;
    else
        return STACK_OUT_OF_MEM;
}

struct StackClass stack_class = {
    sizeof(struct Stack),
    _stack_construct,
    _stack_destruct,
    _stack_size,
    _stack_pop,
    _stack_head,
    _stack_push
};

Stack_t
stack_create(size_t element_size, clib_free_func ff, clib_copy_func cf)
{
    Stack* self = malloc(stack_class.element_sz);
    if (!self)
        return NULL;

    self->klass = &stack_class;
    self->klass->construct(self, element_size, ff, cf);
    if (!self->list) {
        stack_destroy(self);
        return NULL;
    }
    return self;
}

void
stack_destroy(Stack_t stack)
{
    Stack* self = stack;
    StackClass* klass = self->klass;
    
    klass->destruct(self);
}

size_t
stack_size(Stack_t stack)
{
    Stack* self = stack;
    StackClass* klass = self->klass;

    return klass->size(self);
}

void
stack_pop(Stack_t stack)
{
    Stack* self = stack;
    StackClass* klass = self->klass;

    klass->pop(self);
}

void*
stack_head(Stack_t stack)
{
    Stack* self = stack;
    StackClass* klass = self->klass;

    return klass->head(self);    
}

int
stack_push(Stack_t stack, const void* element)
{
    Stack* self = stack;
    StackClass* klass = self->klass;
    
    return klass->push(self, element);
}


