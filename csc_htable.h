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
#include "csc_dlist.h"
#include "csc_basic.h"


/*
 * A entry is a key-value pair.
 * entries with the same hash will be in the same list.
 */
struct csc_htable_entry
{
	//TODO: Support any key type instead of only cstring:
	char * key;
	//Linux style list head node:
	struct csc_dlist list;
};


struct csc_htable
{
	//Number of entries, must be power of 2.
	unsigned size;
	struct csc_htable_entry * table;
	struct csc_dlist list_replace;
	struct csc_dlist list_del;
};


__attribute__ ((unused))
static inline
void csc_htable_init (struct csc_htable * t)
{
	ASSERT_PARAM_NOTNULL (t);
	ASSERT_ISPOW2 (t->size);
	csc_dlist_init (&t->list_replace);
	csc_dlist_init (&t->list_del);
	t->table = calloc (t->size, sizeof (struct csc_htable_entry));
	for (unsigned i = 0; i < t->size; ++i)
	{
		struct csc_dlist * list = &t->table [i].list;
		csc_dlist_init (list);
	}
}


__attribute__ ((unused))
static inline
unsigned csc_htable_hash (unsigned size, char * key)
{
	ASSERT_ISPOW2 (size);
	unsigned h = 0;
	while (*key)
	{
		h = h << 8;
		h += (unsigned)(*key);
		key ++;
	}
	h = h & (size - 1);
	ASSERT (h < size);
	return h;
}


__attribute__ ((unused))
static inline
struct csc_dlist * csc_htable_find_list (struct csc_htable * t, char * key)
{
	ASSERT_PARAM_NOTNULL (t);
	unsigned h = csc_htable_hash (t->size, key);
	//TRACEF ("%s : %i", key, h);
	struct csc_dlist * list = &t->table [h].list;
	struct csc_dlist * p;
	for (p = list->next; p != list; p = p->next)
	{
		struct csc_htable_entry * e = container_of (p, struct csc_htable_entry, list);
		ASSERT (e->key);
		if (strcmp (e->key, key) == 0)
		{
			return p;
		}
	}
	return p;
}


__attribute__ ((unused))
static inline
void csc_htable_set (struct csc_htable * t, struct csc_htable_entry * new)
{
	ASSERT_PARAM_NOTNULL (t);
	unsigned h = csc_htable_hash (t->size, new->key);
	TRACEF ("%s : %i", new->key, h);
	struct csc_dlist * list = &t->table [h].list;
	struct csc_dlist * p;
	for (p = list->next; p != list; p = p->next)
	{
		struct csc_htable_entry * e = container_of (p, struct csc_htable_entry, list);
		ASSERT (e->key);
		if (strcmp (e->key, new->key) == 0)
		{
			csc_dlist_replace (p, &new->list);
			csc_dlist_add_head (&t->list_replace, p);
			//TRACEF ("count %i", csc_dlist_count (list));
			return;
		}
	}
	csc_dlist_add_head (list, &new->list);
	//TRACEF ("count %i", csc_dlist_count (list));
}


__attribute__ ((unused))
static inline
void csc_htable_del (struct csc_htable * t, char * key)
{
	ASSERT_PARAM_NOTNULL (t);
	struct csc_dlist * list;
	list = csc_htable_find_list (t, key);
	if (list->next != list->prev)
	{
		csc_dlist_del (list);
		csc_dlist_add_head (&t->list_del, list);
	}
}













