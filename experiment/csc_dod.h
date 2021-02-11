/* SPDX-License-Identifier: GPL-2.0 */
#pragma once
#include <stdint.h>
#include <stdio.h>

#include "csc_crossos.h"
#include "csc_basic.h"
#include "csc_debug.h"
#include "csc_dlist.h"


#define CSC_EAV_CAPACITY_LIMIT 10000
#define CSC_EAV_ENABLED 0x00000001
/*

 |A|B|C|D|E|F|G|H
0|x| | | | | | |
1|x|x| | | | | |
2|x|x| | | | | |
3|x| | | | | | |
4|x|x|x| | | | |
5|x|x|x| | | | |
6|x| |x| | | | |
7|x| |x| | | | |

*/




struct csc_dod
{
	uint32_t cap;
	uint32_t last;
	uint32_t dim;
	uint32_t stride;
	uint32_t size;
	uint32_t * ents;
	enum csc_type type;
	union
	{
		void * data;
		float * vf32;
		double * vf64;
		uint8_t * vu8;
		uint16_t * vu16;
		uint32_t * vu32;
	};
};



void csc_dod_init(struct csc_dod * dod)
{
	dod->data = calloc (dod->cap * dod->stride * dod->size, 1);
	dod->ents = calloc (dod->cap, sizeof (uint32_t));
	ASSERT_NOTNULL (dod->data);
	ASSERT_NOTNULL (dod->ents);
}



void csc_dod_add(struct csc_dod * dod, uint32_t e, void * value)
{
	uint8_t * p = dod->data;
	p += dod->last * dod->stride;
	memcpy (p, value, dod->size);
	dod->ents[dod->last] = e;
	dod->last++;
}










