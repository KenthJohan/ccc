/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once
#include <stdint.h>
#include <stdio.h>

#include "csc_crossos.h"
#include "csc_basic.h"
#include "csc_debug.h"
#include "csc_dlist.h"
#include "csc_store.h"



struct csc_store_u32
{
	uint32_t cap;
	struct csc_dlist h1; //Available list
	struct csc_dlist h2; //Taken list
	struct csc_dlist * nodes; //Memory
	uint32_t * v; //Memory
};


void csc_store_u32_init (struct csc_store_u32 * s)
{
	csc_dlist_init (&s->h1);
	csc_dlist_init (&s->h2);
	s->nodes = malloc (sizeof (struct csc_dlist) * s->cap);
	s->v = malloc (sizeof (uint32_t) * s->cap);
	csc_dlist_addv (&s->h1, s->nodes, s->cap);
}


int csc_store_u32_add (struct csc_store_u32 * s, uint32_t value)
{
	struct csc_dlist * h1 = &(s->h1); //List of free ids
	struct csc_dlist * h2 = &(s->h2); //List of free ids
	struct csc_dlist * node = h1->next;
	if (csc_dlist_empty (h1)){return -1;}
	ptrdiff_t i = node - s->nodes;
	//printf ("ptrdiff_t %i\n", (int)i);
	ASSERT (csc_dlist_nonempty (h1));
	ASSERT (i >= 0);
	ASSERT (i < s->cap);
	csc_dlist_move_head (h2, node);
	s->v[i] = value;
	return 0;
}


int csc_store_u32_remove (struct csc_store_u32 * s, uint32_t * value)
{
	struct csc_dlist * h1 = &(s->h1); //List of free ids
	struct csc_dlist * h2 = &(s->h2); //List of free ids
	struct csc_dlist * node = h2->next;
	if (csc_dlist_empty (h2)){return -1;}
	ptrdiff_t i = node - s->nodes;
	//printf ("ptrdiff_t %i\n", (int)i);
	ASSERT (csc_dlist_nonempty (h2));
	ASSERT (i >= 0);
	ASSERT (i < s->cap);
	csc_dlist_move_head (h1, node);
	(*value) = s->v[i];
	return 0;
}



struct csc_as
{
	uint32_t cap;
	uint32_t last;
};


void csc_as_init (struct csc_as * as)
{
	as->last = 0;
}


int csc_as_push (struct csc_as * as)
{
	uint32_t cap = as->cap;
	uint32_t last = as->last;
	if (last == cap)
	{
		cap = cap * 2;
	}
	as->last += 1;
	return 0;
}


int csc_as_pop (struct csc_as * as)
{
	as->last -= 1;
	return 0;
}


void csc_as_relloc (struct csc_as * as, uint32_t size, void ** memory)
{
	void * mem = realloc (memory, as->cap);
	ASSERT (mem);
	*memory = mem;
}


void test33()
{
	struct sys1
	{
		struct csc_as as;
		uint32_t * flags;
		uint32_t * masks;
	};
	struct sys1 s1;
	if (csc_as_push (&s1.as))
	{
		csc_as_relloc (&s1.as, sizeof (uint32_t), &s1.flags);
		csc_as_relloc (&s1.as, sizeof (uint32_t), &s1.masks);
	};
	s1.flags[s1.as.last] = 1;
	s1.masks[s1.as.last] = 1;
}


















