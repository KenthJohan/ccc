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
static inline void csc_dlist_add_head (struct csc_dlist *head, struct csc_dlist *new)
{
	ASSERT_PARAM_NOTNULL (head);
	ASSERT_PARAM_NOTNULL (new);
	csc_dlist_add_between (head, new, head->next);
}


__attribute__ ((unused))
static inline void csc_dlist_add_tail (struct csc_dlist *head, struct csc_dlist *new)
{
	ASSERT_PARAM_NOTNULL (head);
	ASSERT_PARAM_NOTNULL (new);
	csc_dlist_add_between (head->prev, new, head);
}


static inline void csc_dlist_connect (struct csc_dlist * a, struct csc_dlist * b)
{
	ASSERT_PARAM_NOTNULL (a);
	ASSERT_PARAM_NOTNULL (b);
	a->prev = b;
	b->next = a;
}


__attribute__ ((unused))
static inline void csc_dlist_del (struct csc_dlist * entry)
{
	ASSERT_PARAM_NOTNULL (entry);
	csc_dlist_connect (entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}


__attribute__ ((unused))
static inline unsigned csc_dlist_count (struct csc_dlist * entry)
{
	ASSERT_PARAM_NOTNULL (entry);
	unsigned count = 0;
	struct csc_dlist * p;
	for (p = entry->next; p != entry; p = p->next)
	{
		count ++;
	}
	return count;
}


__attribute__ ((unused))
static inline void csc_dlist_replace (struct csc_dlist *old, struct csc_dlist *new)
{
	ASSERT_PARAM_NOTNULL (old);
	ASSERT_PARAM_NOTNULL (new);
	ASSERT_PARAM_NOTNULL (new->next);
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}


__attribute__ ((unused))
static inline int csc_dlist_empty (struct csc_dlist const *head)
{
	ASSERT_PARAM_NOTNULL (head);
	return head->next == head;
}


__attribute__ ((unused))
static inline int csc_dlist_nonempty (struct csc_dlist const *head)
{
	ASSERT_PARAM_NOTNULL (head);
	return head->next != head;
}


__attribute__ ((unused))
static inline void csc_dlist_init_v (struct csc_dlist *list, unsigned n)
{
	ASSERT_PARAM_NOTNULL (list);
	while (n--)
	{
		csc_dlist_init (list+n);
	}
}


__attribute__ ((unused))
static inline unsigned csc_dlist_nonempty_v (struct csc_dlist *list, unsigned n)
{
	ASSERT_PARAM_NOTNULL (list);
	unsigned m = 0;
	while (n--)
	{
		if (csc_dlist_nonempty (list + n))
		{
			m ++;
		}
	}
	return m;
}


__attribute__ ((unused))
static inline unsigned csc_dlist_empty_v (struct csc_dlist *list, unsigned n)
{
	ASSERT_PARAM_NOTNULL (list);
	unsigned m = 0;
	while (n--)
	{
		if (csc_dlist_empty (list + n))
		{
			m ++;
		}
	}
	return m;
}


__attribute__ ((unused))
static inline int csc_dlist_cmp_ab (char const * s, char const * a, char const * b)
{
	int d = 0;
	while (1)
	{
		if (a >= b) {break;}
		d += s[0] - a[0];
		if (s[0] == '\0') {break;}
		if (a[0] == '\0') {break;}
		s++;
		a++;
	}
	return d;
}


__attribute__ ((unused))
static inline int csc_dlist_find_str (struct csc_dlist *entry, struct csc_dlist *base, char const * needle, char const * needle_end, char const * str, unsigned stride, unsigned offset)
{
	ASSERT_PARAM_NOTNULL (entry);
	ASSERT_PARAM_NOTNULL (needle);
	struct csc_dlist * p;
	for (p = entry->next; p != entry; p = p->next)
	{
		ptrdiff_t i = p - base;
		char const * s = str + offset + (i * stride);
		if (needle_end && (csc_dlist_cmp_ab (s, needle, needle_end) == 0))
		{
			return (int)i;
		}
		else if (strcmp (s, needle) == 0)
		{
			return (int)i;
		}
	}
	return -1;
}
