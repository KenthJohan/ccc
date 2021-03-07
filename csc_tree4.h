/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "csc_debug.h"

/*
https://en.wikipedia.org/wiki/Left-child_right-sibling_binary_tree
TODO:
	This is currently used for AST. Can this be more generic?
*/
struct csc_tree4;
struct csc_tree4
{
	struct csc_tree4 * prev;
	struct csc_tree4 * next;
	struct csc_tree4 * parent;
	struct csc_tree4 * child;
	int child_count;
};


void csc_tree4_addchild (struct csc_tree4 * parent, struct csc_tree4 * child)
{
	ASSERT_PARAM_NOTNULL (parent);
	ASSERT_PARAM_NOTNULL (child);
	child->prev = 0;
	child->next = parent->child;
	child->parent = parent;
	if (parent->child)
	{
		parent->child->prev = child;
	}
	parent->child = child;
	parent->child_count++;
}


/*
 * Insert a new entry (sibling1) after the specified head (sibling0).
 * @sibling0: list head to add it after
 * @sibling1: new entry to be added
 *   R           R
 *  / \   -->   / \
 * C   s0      C   s0
 *                  \
 *                  s1
 * Node R and C are there only for visuals.
 */
void csc_tree4_addsibling (struct csc_tree4 * sibling0, struct csc_tree4 * sibling1)
{
	ASSERT_PARAM_NOTNULL (sibling0);
	ASSERT_PARAM_NOTNULL (sibling1);
	sibling0->next = sibling1;
	sibling1->prev = sibling0;
	sibling1->parent = sibling0->parent;
	if (sibling0->parent)
	{
		sibling0->parent->child_count++;
	}
}


void csc_tree4_addparent (struct csc_tree4 * node, struct csc_tree4 * newnode)
{
	ASSERT_PARAM_NOTNULL (node);
	ASSERT_PARAM_NOTNULL (newnode);
	newnode->child = node;
	newnode->parent = node->parent;
	newnode->next = node->next;
	newnode->prev = node->prev;
	newnode->child_count = 1;
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


/*
 * Delete a list entry by making the prev/next and parent/child entries
 * point to each other.
 */
void csc_tree4_remove (struct csc_tree4 * node)
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


void csc_tree4_addparent2 (struct csc_tree4 * node, struct csc_tree4 * newnode)
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
struct csc_tree4 const *
csc_tree4_traverse
(struct csc_tree4 const * node, int *depth)
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
