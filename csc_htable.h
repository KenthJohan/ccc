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
	union
	{
		void * pval;
		intmax_t ival;
		uintmax_t uval;
	};
	//Linux style list head node:
	struct csc_dlist list;
};


struct csc_htable
{
	//Number of entries, must be power of 2.
	unsigned size;
	struct csc_htable_entry * table;
};


__attribute__ ((unused))
static inline
void csc_htable_init (struct csc_htable * t)
{
	ASSERT_PARAM_NOTNULL (t);
	ASSERT_ISPOW2 (t->size);
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
struct csc_htable_entry * csc_htable_find (struct csc_htable * t, char * key)
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
			return e;
		}
	}
	return NULL;
}


__attribute__ ((unused))
static inline
void csc_htable_del (struct csc_htable * t, char * key)
{
	ASSERT_PARAM_NOTNULL (t);
	struct csc_htable_entry * e;
	e = csc_htable_find (t, key);
	if (e)
	{
		csc_dlist_del (&e->list);
		ASSERT (e->key);
		free (e->key);
		e->key = NULL;
	}
}


/*
 * Finds the entry by parameter key.
 * If the entry can not be found then allocate a new one and add it to the table.
 * Returns a new entry or found entry.
 */
__attribute__ ((unused))
static inline
struct csc_htable_entry * csc_htable_acquire (struct csc_htable * t, char * key)
{
	ASSERT_PARAM_NOTNULL (t);
	struct csc_htable_entry * e;
	e = csc_htable_find (t, key);
	if (e == NULL)
	{
		unsigned h = csc_htable_hash (t->size, key);
		struct csc_dlist * list = &t->table [h].list;
		e = calloc (1, sizeof (struct csc_htable_entry));
		ASSERT (e);
		e->key = strdup (key);
		csc_dlist_add_head (&e->list, list);
	}
	return e;
}


__attribute__ ((unused))
static inline
void csc_htable_set_ptr (struct csc_htable * t, char * key, void * val)
{
	ASSERT_PARAM_NOTNULL (t);
	struct csc_htable_entry * e = csc_htable_acquire (t, key);
	e->pval = val;
}


__attribute__ ((unused))
static inline
void csc_htable_set_imax (struct csc_htable * t, char * key, intmax_t val)
{
	ASSERT_PARAM_NOTNULL (t);
	struct csc_htable_entry * e = csc_htable_acquire (t, key);
	e->ival = val;
}


__attribute__ ((unused))
static inline
void csc_htable_set_umax (struct csc_htable * t, char * key, uintmax_t val)
{
	ASSERT_PARAM_NOTNULL (t);
	struct csc_htable_entry * e = csc_htable_acquire (t, key);
	e->uval = val;
}
















