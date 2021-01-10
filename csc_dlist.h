/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include "csc_debug.h"
#include "csc_basic.h"
#include "csc_str.h"

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
	ASSERT (head == head->next->prev);
	ASSERT (new == new->next->prev);
}


__attribute__ ((unused))
static inline void csc_dlist_add_tail (struct csc_dlist *head, struct csc_dlist *new)
{
	ASSERT_PARAM_NOTNULL (head);
	ASSERT_PARAM_NOTNULL (new);
	csc_dlist_add_between (head->prev, new, head);
	ASSERT (head == head->next->prev);
	ASSERT (new == new->next->prev);
}


static inline void csc_dlist_addv (struct csc_dlist *head, struct csc_dlist new[], uint32_t n)
{
	ASSERT_PARAM_NOTNULL (head);
	ASSERT_PARAM_NOTNULL (new);
	for (uint32_t i = 0; i < n; ++i)
	{
		csc_dlist_add_tail (head, new + i);
	}
}


static inline void csc_dlist_connect (struct csc_dlist * a, struct csc_dlist * b)
{
	ASSERT_PARAM_NOTNULL (a);
	ASSERT_PARAM_NOTNULL (b);
	a->next = b;
	b->prev = a;
	ASSERT (a == a->next->prev);
	ASSERT (b == b->next->prev);
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
static inline void csc_dlist_swap (struct csc_dlist * a, struct csc_dlist * b)
{
	/*
	ASSERT_PARAM_NOTNULL (a);
	ASSERT_PARAM_NOTNULL (b);
	a->prev->next = b;
	a->next->prev = b;
	b->prev->next = a;
	b->next->prev = a;
	SWAP (struct csc_dlist *, a->next, b->next);
	SWAP (struct csc_dlist *, a->prev, b->prev);
	*/
}


__attribute__ ((unused))
static inline void csc_dlist_move_head (struct csc_dlist * head, struct csc_dlist * node)
{
	csc_dlist_del (node);
	csc_dlist_add_head (head, node);
}


__attribute__ ((unused))
static inline void csc_dlist_move_tail (struct csc_dlist * node, struct csc_dlist * head)
{
	csc_dlist_del (node);
	csc_dlist_add_tail (head, node);
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
	ASSERT (new == new->next->prev);
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
static inline int csc_dlist_find_str (struct csc_dlist *entry, struct csc_dlist *base, char const * needle, char const * needle_end, char const * str, unsigned stride, unsigned offset)
{
	ASSERT_PARAM_NOTNULL (entry);
	ASSERT_PARAM_NOTNULL (needle);
	struct csc_dlist * p;
	for (p = entry->next; p != entry; p = p->next)
	{
		ptrdiff_t i = p - base;
		char const * s = str + offset + (i * stride);
		if (csc_str_cmp (s, needle, NULL, needle_end) == 0)
		{
			return (int)i;
		}
	}
	return -1;
}









