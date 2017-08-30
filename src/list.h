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

#ifndef LIST_H
#define LIST_H

typedef void* List_t;

typedef struct ListNode {
    struct ListNode*    next;
    void*               data;
} ListNode;

typedef void  (*list_free_func)(void* element);
typedef void* (*list_copy_func)(void* dest, const void* src, size_t n);
typedef int   (list_cmp_func)(const void* k1, const void* k2);

/**
 * create an empty list.
 *
 * creates an empty list capable to grow and shrink when neccesarry
 * 
 * @param element_size [in] the sizeof() an single element.
 * @param ff [in] the free func will be called when individual elements
 *                are erased from the list.
 * @param cf [in] the function used to copy an element into the list.
 *                if none is specified memcpy will be used.
 */
List_t list_create(size_t element_size, list_free_func ff, list_copy_func cf);

/**
 * Destroys the list and frees all members.
 */
void   list_destroy(List_t list);

/**
 * Returns the number of instances stored in the list.
 */
size_t list_size(const List_t list);

/**
 * Prepends to the start of the list.
 *
 * This is a constant time and efficient operation.
 *
 * @param [in] list the list to append to.
 * @param [in] value, a value to append to the list.
 * @returns a newly inserted node or NULL 
 */
ListNode* list_prepend(List_t list, const void* value);


/**
 * Appends to the end of the list.
 *
 * Optionally start may be provided, it should be a member of the current list,
 * if this node is chosen, one can prevent iterating from all the way to the
 * end.
 *
 * @param [in] list the list to append to.
 * @param [in] start, NULL, or a node from the list as a start hint.
 * @param [in] value, a value to append to the list.
 */
ListNode* list_append(List_t list, ListNode* start, const void* value);

/**
 * Inserts a value in the list.
 *
 * Inserts a value to the list before another node. Make sure that
 * node is in the list, one can always insert before NULL, the end
 * of the list.
 *
 * @param [in] list the list to append to.
 * @param [in] before, NULL inserts at the start or before "before".
 * @param [in] value, a value to insert into the list.
 */
ListNode* list_insert(List_t list, ListNode* before, const void* value);

/**
 * Inserts a value in the list.
 *
 * Inserts a value to the list after another node. Make sure that
 * node is in the list
 *
 * @param [in] list the list to append to.
 * @param [in] after non null value of item in the list.
 * @param [in] value a value to insert into the list.
 */
ListNode* list_insert_after(List_t list, ListNode* after, const void* v);

/**
 * Removes a node from the list.
 *
 * removes one node from the list.
 *
 * @param [in] list the list to append to.
 * @param [in] node non null value of item in the list.
 */
void list_remove(List_t list, ListNode* node);

/**
 * Remove an node from the list.
 *
 * Removes nodes starting from begin until and excluding end or NULL is
 * encountered.
 *
 * @param [in] list the list to append to.
 * @param [in] node non null value of item in the list.
 */
void list_remove_range(List_t list, ListNode* begin, ListNode* end);

/**
 * Finds a node in a list, if the node isn't found NULL is returned.
 */
ListNode* list_find(const List_t list,
                    const void* value,
                    list_cmp_func cmpf
                    );

/**
 * Reverses a list in place.
 */
void list_reverse(List_t list);

/**
 * Returns the head of the list.
 */
ListNode* list_begin(const List_t list);

/**
 * Compares two lists.
 *
 * @param cmp a list_cmp_func that returns  a value smaller, a value
 * larger than or a value equal to 0, for the case when the value from
 * the first list is greater, smaller or equal to the one in the other
 * list.
 *
 * @returns a value from cmp or something based on the size of the lists.
 *          when the return value is 0, they are equal.
 */
int list_compare(const List_t l1, const List_t l2, list_cmp_func cmp);

#endif /* ifndef LIST_H*/
