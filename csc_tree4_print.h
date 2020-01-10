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

#include <stdio.h>
#include "csc_tcol.h"
#include "csc_tree4.h"



void csc_tree4_print_traverse
(
struct csc_tree4 const * node,
void * ptr,
char const * (*cb)(struct csc_tree4 const * node, void *ptr)
)
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
		puts (cb (node, ptr));
		node = csc_tree4_traverse (node, &depth);
	}
}
