/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <stdio.h>
#include "csc_tcol.h"
#include "csc_tree4.h"

typedef void (*csc_tree4_print_traverse_cb)(struct csc_tree4 const * node, void *ptr);


/**
 * @brief Print pretty left-child right-sibling binary tree.
 * @param node Traverse the tree from this node downwards.
 * @param ptr User pointer that is passed on to the callback function \p cb.
 * @param cb A callback function that is called for each node in the tree.
 */
void csc_tree4_print_traverse (struct csc_tree4 const * node, void * ptr, csc_tree4_print_traverse_cb cb)
{
	char vertical[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u2502" TCOL_RST;
	char cross[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u251C" TCOL_RST;
	char corner[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u2514" TCOL_RST;
	char dash[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u2500" TCOL_RST;
	int depth = 0;
	//int leaf = 0;
	uint32_t indent = 0;
	while (node)
	{
		for (int i = 0; i < depth; i ++)
		{
			if (indent & (1 << i))
			{
				printf ("%s", vertical);
			}
			else
			{
				putc (' ', stdout);
			}
			putc (' ', stdout);
			putc (' ', stdout);
		}
		if (node->next)
		{
			printf ("%s", cross);
			indent |= (1 << depth);
		}
		if (node->next == NULL)
		{
			printf ("%s", corner);
			indent &= ~(1 << depth);
		}
		printf ("%s", dash);
		cb (node, ptr);
		node = csc_tree4_traverse (node, &depth);
	}
}
