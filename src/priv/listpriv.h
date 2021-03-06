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

#ifndef LISTPRIV_H
#define LISTPRIV_H

#include <stdlib.h>
#include "../list.h"

struct ListClass;

//struct ListNode {
//    struct ListNode*    next;
//    void*               data;
//};

struct List {
    struct ListClass*   klass;
    struct ListNode*    head;
    size_t              elem_size;
    size_t              nelements;
    list_free_func      ff;
    list_copy_func      cf;
};

#endif /*LISTPRIV_H*/
