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

#include <stdint.h>
#include <stddef.h>
#include "csc_debug.h"
#include "csc_dlist.h"


/*
https://en.wikipedia.org/wiki/Left-child_right-sibling_binary_tree
TODO:
	This is currently used for AST. Can this be more generic?
*/
struct csc_pcstree;
struct csc_pcstree
{
	struct csc_pcstree * prev;
	struct csc_pcstree * next;
	struct csc_pcstree * parent;
	struct csc_pcstree * child;
	int child_count;
	char const * ptr;
};


void csc_pcstree_initsibling (struct csc_pcstree * tree)
{
	ASSERT_PARAM_NOTNULL (tree);
	tree->prev = tree;
	tree->next = tree;
}


struct csc_pcstree * csc_pcstree_malloc()
{
	struct csc_pcstree * node = malloc (sizeof (struct csc_pcstree) * 2);
	node [0].child_count = 0;
	node [0].ptr = NULL;
	node [0].child = node + 1;
	node [0].parent = NULL;
	node [1].child_count = 0;
	node [1].ptr = NULL;
	node [1].child = NULL;
	node [1].parent = node + 0;
	csc_pcstree_initsibling (node + 0);
	csc_pcstree_initsibling (node + 1);
	return node;
}


static inline void csc_pcstree_addbetween (struct csc_pcstree *prev, struct csc_pcstree *new, struct csc_pcstree *next)
{
	ASSERT_PARAM_NOTNULL (prev);
	ASSERT_PARAM_NOTNULL (new);
	ASSERT_PARAM_NOTNULL (next);
	new->next = next;
	new->prev = prev;
	next->prev = new;
	prev->next = new;
}


/*
+-ROOT
| +-S0
| +-S1
*/
void csc_pcstree_addsibling (struct csc_pcstree * head, struct csc_pcstree * new)
{
	ASSERT_PARAM_NOTNULL (head);
	ASSERT_PARAM_NOTNULL (new);
	new->parent = head->parent;
	csc_pcstree_addbetween (head, new, head->next);
}


void csc_pcstree_addchild (struct csc_pcstree * head, struct csc_pcstree * new)
{
	ASSERT_PARAM_NOTNULL (head);
	ASSERT_PARAM_NOTNULL (new);
	ASSERT (head->child != NULL);
	csc_pcstree_addsibling (head->child, new);
}


/*
+-S0
| +-S00
| +-head
| +-S01
+-S1

+-S0
| +-S00
| +-new
| +-S01
+-S1


+-S0
| +-S00
| +-S01
+-S1

+-S0
| +-new
| +-S00
| +-S01
+-S1
*/
void csc_pcstree_replace (struct csc_pcstree * old, struct csc_pcstree * new)
{
	ASSERT_PARAM_NOTNULL (old);
	ASSERT_PARAM_NOTNULL (new);
	ASSERT (old->child);
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
	new->parent = old->parent;
	//new->parent->child = new;//Do not do this
	new->child = old->child;
	new->child->parent = new;
	old->next = NULL;
	old->prev = NULL;
	old->parent = NULL;
	//old->child = NULL;//Do not erase this info
}




/*
+-S0
| +-head
+-S1

+-S0
| +-new
|   +-head
+-S1
*/
void csc_pcstree_addparent (struct csc_pcstree * head, struct csc_pcstree * new)
{
	ASSERT_PARAM_NOTNULL (head);
	ASSERT_PARAM_NOTNULL (new);
	csc_pcstree_replace (head, new);
	csc_pcstree_addchild (new, head);
}


/*
 * Delete a list entry by making the prev/next and parent/child entries
 * point to each other.
 */
void csc_pcstree_remove (struct csc_pcstree * node)
{
	ASSERT_PARAM_NOTNULL (node);
	if (node->next)
	{
		node->next->prev = node->prev;
	}
	if (node->prev)
	{
		node->prev->next = node->next;
	}
	if (node->parent && (node->parent->child == node))
	{
		node->parent->child = node->next;
	}
	if (node->child)
	{
		node->child->parent = 0;
	}
	if (node->parent)
	{
		node->parent->child_count--;
	}
}


void csc_pcstree_addparent2 (struct csc_pcstree * node, struct csc_pcstree * newnode)
{
	ASSERT_PARAM_NOTNULL (node);
	ASSERT_PARAM_NOTNULL (newnode);
	newnode->child = node;
	newnode->parent = node->parent;
	newnode->next = node->next;
	newnode->prev = node->prev;
	if (node->next)
	{
		node->next->prev = newnode;
	}
	if (node->prev)
	{
		node->prev->next = newnode;
	}
	if (node->parent && (node->parent->child == node))
	{
		node->parent->child = newnode;
	}
	node->parent = newnode;
	node->next = 0;
	node->prev = 0;
}


__attribute__ ((unused))
static inline
struct csc_pcstree const *
csc_pcstree_traverse
(struct csc_pcstree const * node, int *depth)
{
	if (node == NULL)
	{
		return node;
	}
	else if (node->child)
	{
		(*depth) += 1;
		return node->child;
	}
	else if (node->next)
	{
		return node->next;
	}
	else if (node->parent && (node->child == NULL) && (node->next == NULL))
	{
		do
		{
			(*depth) -= 1;
			node = node->parent;
		}
		while (node && (node->next == NULL));
		if (node && node->next)
		{
			return node->next;
		}
	}
	return NULL;
}
