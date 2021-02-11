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


void ** p;
p[A] = 0,1,2,3,4,5,6,7
p[B] = 1,2,4,5
p[C] = 4,5,6,7



0 | A |

1 | A |
1 | B |

2 | A |
2 | B |

3 | A |

4 | A |
4 | B |
4 | C |

5 | A |
5 | B |
5 | C |

6 | A |
6 | C |

7 | A |
7 | C |






*/



struct csc_eav_sparse
{
	uint32_t capacity;
	uint32_t * flags;
	uint32_t * e;
	uint32_t * a;
	void ** v;
};


struct csc_eav_entities
{
	uint32_t capacity;
	uint32_t * flags;
};


struct csc_eav_attributes
{
	uint32_t capacity;
	uint32_t * flags;
	uint32_t * size;

	uint32_t * count;
	void ** data;
};


struct csc_eav
{
	struct csc_eav_entities entities;
	struct csc_eav_attributes attributes;
	struct csc_eav_sparse sparse;
};


uint32_t csc_eav_init (struct csc_eav * eav)
{
	ASSERT (eav->entities.capacity < CSC_EAV_CAPACITY_LIMIT);
	ASSERT (eav->attributes.capacity < CSC_EAV_CAPACITY_LIMIT);
	ASSERT (eav->sparse.capacity < CSC_EAV_CAPACITY_LIMIT);
	eav->entities.flags = calloc (eav->entities.capacity, sizeof (uint32_t));
	eav->attributes.flags = calloc (eav->attributes.capacity, sizeof (uint32_t));
	eav->attributes.size = calloc (eav->attributes.capacity, sizeof (uint32_t));
	eav->attributes.count = calloc (eav->attributes.capacity, sizeof (uint32_t));
	eav->attributes.data = calloc (eav->attributes.capacity, sizeof (void*));
	eav->sparse.flags = calloc (eav->sparse.capacity, sizeof (uint32_t));
	eav->sparse.e = calloc (eav->sparse.capacity, sizeof (uint32_t));
	eav->sparse.a = calloc (eav->sparse.capacity, sizeof (uint32_t));
	eav->sparse.v = calloc (eav->sparse.capacity, sizeof (void*));
	ASSERT (eav->entities.flags);
	ASSERT (eav->attributes.flags);
	ASSERT (eav->attributes.size);
	ASSERT (eav->sparse.e);
	ASSERT (eav->sparse.a);
	ASSERT (eav->sparse.v);
}


void csc_eav_set (struct csc_eav * eav, uint32_t e, uint32_t a, void * value)
{

}


void * csc_eav_get (struct csc_eav * eav, uint32_t e, uint32_t a)
{
	return NULL;
}

void * csc_eav_disable (struct csc_eav * eav, uint32_t e)
{
	eav->entities.flags[e] &= ~CSC_EAV_ENABLED;
}











