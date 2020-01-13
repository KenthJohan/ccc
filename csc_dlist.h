/*
MIT License

Copyright (c) 2019 CSC Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "csc_debug.h"

struct csc_dlist;
struct csc_dlist
{
	struct csc_dlist * prev;
	struct csc_dlist * next;
};


__attribute__ ((unused))
static inline void csc_dlist_init (struct csc_dlist *list)
{
	ASSERT_PARAM_NOTNULL (list);
	list->prev = list;
	list->next = list;
}


static inline void csc_dlist_add_between (struct csc_dlist *prev, struct csc_dlist *new, struct csc_dlist *next)
{
	ASSERT_PARAM_NOTNULL (prev);
	ASSERT_PARAM_NOTNULL (new);
	ASSERT_PARAM_NOTNULL (next);
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}


__attribute__ ((unused))
static inline void csc_dlist_add_head (struct csc_dlist *new, struct csc_dlist *head)
{
	ASSERT_PARAM_NOTNULL (head);
	csc_dlist_add_between (head, new, head->next);
}


__attribute__ ((unused))
static inline void csc_dlist_add_tail (struct csc_dlist *new, struct csc_dlist *head)
{
	ASSERT_PARAM_NOTNULL (head);
	csc_dlist_add_between (head->prev, new, head);
}


static inline void csc_dlist_connect (struct csc_dlist * a, struct csc_dlist * b)
{
	a->prev = b;
	b->next = a;
}


__attribute__ ((unused))
static inline void csc_dlist_del (struct csc_dlist * entry)
{
	csc_dlist_connect (entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}




