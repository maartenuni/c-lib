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

#include "priv/listpriv.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct ListClass {
    size_t  elem_size;
    void  (*construct)(struct List*, size_t, list_free_func, list_copy_func);
    void  (*destruct)(struct List* self);
    size_t(*size)(const struct List* self);
    ListNode* (*prepend)
        (struct List* self, const void* value);
    ListNode* (*append)
        (struct List* self, ListNode* start, const void* value);
    ListNode* (*insert)
        (struct List* self, ListNode* before, const void* value);
    ListNode* (*insert_after)
        (struct List* self, ListNode* after, const void* value);
    void (*remove) (struct List* self, ListNode* node);
    void (*remove_range)
        (struct List* self, ListNode* begin, ListNode* end);
    ListNode* (*find)(struct List* self, const void* value, list_cmp_func cmp);
    ListNode* (*begin)(const struct List* self);
    void (*reverse)(struct List* self);
    int  (*compare)
        (const struct List* l1, const struct List* l2, list_cmp_func cmp);
};

typedef struct ListClass ListClass;

static ListNode*
list_node_create(struct List* self, const void* value)
{
    ListNode* newnode = calloc(1, sizeof(ListNode));
    void* data = malloc(self->elem_size);
    if (!data || ! newnode) {
        free(newnode);
        free(data);
        return NULL;
    }
    else {
        newnode->data = data;
        self->cf(data, value, self->elem_size);
        return newnode;
    }
}

static void 
list_node_destroy(struct List* self, ListNode* node)
{
    self->ff(node->data);
    free(node);
}

struct ListClass list_class;

static void
list_constructor(struct List* self,
                 size_t element_sz,
                 list_free_func ff,
                 list_copy_func cf
                 )
{
    assert(self);
    if (!ff)
        self->ff = free;
    else
        self->ff = ff;

    if (!cf)
        self->cf = memcpy;
    else
        self->cf = cf;

    self->elem_size = element_sz;
    
    // are already calloc-ed to 0
    // self->nelements = 0;
}

static void
list_destructor(struct List* self)
{
    if (!self)
        return;

    struct ListNode* pnode = self->head;
    while (pnode) {
        struct ListNode* temp = pnode;
        pnode = pnode->next;
        self->ff(temp->data);
        free(temp);
    }
    free(self);
}

static size_t 
_list_size(const struct List* self)
{
    return self->nelements;
}

static ListNode*
_list_prepend(struct List* self, const void* value)
{
    ListNode* newnode = list_node_create(self, value);
    if (!newnode)
        return NULL;

    newnode->next = self->head;
    self->nelements++;
    self->head = newnode;
    return newnode;
}

static ListNode*
_list_append(struct List* self, ListNode* start, const void* value)
{
    if (self->head == NULL)
        return self->klass->insert(self, NULL, value);
    else {
        ListNode* newnode = list_node_create(self, value);
        if (!newnode )
            return NULL;
        while (start->next != NULL)
            start = start->next;
        start->next = newnode;
        self->nelements++;
        return newnode;
    }
}

static ListNode*
_list_insert(struct List* self, ListNode* before, const void* value)
{
    ListNode* head, *newnode = list_node_create(self, value);
    if (!newnode)
        return NULL;

    newnode->next = before;

    head = self->head;
    while(head && head->next != before)
        head = head->next;

    if (head)
        head->next = newnode;
    else
        self->head = newnode;
    self->nelements++;
    return newnode;
}

static ListNode*
_list_insert_after(struct List* self, ListNode* after, const void* value)
{
    assert(after != NULL);
    assert(value);
    ListNode *newnode = list_node_create(self, value);
    if (!newnode)
        return NULL;
    
    newnode->next = after->next;
    after->next = newnode;
    self->nelements++;

    return newnode;
}

static void 
_list_remove(struct List* self, ListNode* node)
{
    self->klass->remove_range(self, node, node->next);
}

static void
_list_remove_range(struct List* self, ListNode* begin, ListNode* end)
{
    size_t n_rm =  0;
    ListNode **b, *e;
    b = &self->head;
    while((*b) != begin)
        b = &(*b)->next;

    e = *b;
    while(e != end) {
        ListNode* rm = e;
        e = e->next;
        list_node_destroy(self, rm);
        n_rm++;
    }
    *b = e;
    self->nelements -= n_rm;
}

static ListNode*
_list_find(struct List* self, const void* value, list_cmp_func cmp)
{
    const ListNode* head = self->head;
    while(head) {
        if (cmp(head->data, value) == 0)
            break;
        head = head->next;
    }
    return (ListNode*)head;
}

static ListNode*
_list_begin(const struct List* self)
{
    return self->head;
}

static void
_list_reverse(struct List* self)
{
    ListNode* head = self->head, *newend = NULL;
    while (head) {
        ListNode* next  = head->next;
        head->next      = newend;
        newend          = head;
        head            = next;
    }
    self->head = newend;
}

static int
_list_compare(const struct List* l1, const struct List* l2, list_cmp_func cmp)
{
    int ret = l1->nelements - l2->nelements;
    if (ret)
        return ret;
    ListNode* n1 = l1->head, *n2 = l2->head;

    while (n1 && n2) {
        if ((ret = cmp(n1->data, n2->data)) != 0)
            return ret;
        n1 = n1->next, n2 = n2->next;
    }
    return ret;
}

struct ListClass list_class = {
    sizeof(struct List),
    list_constructor,
    list_destructor,
    _list_size,
    _list_prepend,
    _list_append,
    _list_insert,
    _list_insert_after,
    _list_remove,
    _list_remove_range,
    _list_find,
    _list_begin,
    _list_reverse,
    _list_compare
};

List_t list_create(size_t element_sz, list_free_func ff, list_copy_func cf)
{
    struct List* self = calloc(1, sizeof(struct List));
    if (!self)
        return self;

    self->klass = &list_class;
    self->klass->construct(self, element_sz, ff, cf);
    return self;
}

void list_destroy(List_t self)
{
    struct List* this = self;
    struct ListClass* klass = this->klass;

    klass->destruct(this);
}

size_t list_size(const List_t self)
{
    struct List* this = self;
    struct ListClass* klass = this->klass;

    return klass->size(this);
}

ListNode* list_prepend(List_t self, const void* value)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    return klass->prepend(self, value);
}

ListNode* list_append(List_t self, ListNode* start, const void* value)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    return klass->append(self, start, value);
}

ListNode* list_insert(List_t self, ListNode* before, const void* value)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    return klass->insert(self, before, value);
}

ListNode* list_insert_after(List_t self, ListNode* after, const void* value)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    return klass->insert_after(self, after, value);
}

void list_remove(List_t self, ListNode* node) {
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    klass->remove(self, node);
}

void list_remove_range(List_t self, ListNode* begin, ListNode* end) {
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    klass->remove_range(self, begin, end);
}

ListNode* list_find(List_t self, const void* value, list_cmp_func cmp)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    return klass->find(self, value, cmp);
}

ListNode* list_begin(const List_t self)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    return klass->begin(self);
}

void list_reverse(List_t self)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    klass->reverse(self);
}

int list_compare(List_t self, List_t l2, list_cmp_func cmp)
{
    struct List* this = (struct List*) self;
    ListClass* klass = this->klass;

    return klass->compare(self, l2, cmp);
}
