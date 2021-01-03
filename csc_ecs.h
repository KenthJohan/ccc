#pragma once
#include <stdint.h>
#include <stdio.h>

#include "csc_crossos.h"
#include "csc_basic.h"
#include "csc_debug.h"
#include "csc_dlist.h"
#include "csc_store.h"


/*



Components
|I|Name|
|0|c1  |
|1|c2  |
|2|c3  |


|Ent| Chunk | ChunkI |
| 0 | 0     | 0
| 1 | 1     | 2
| 2 | 0     | 3
| 3 | 0     | 3
| 4 | 1     | 0
| 5 | 1     | 1
| 6 | 0     | 1
| 7 | 1     | 3

|Chunk|Mask | Entcount
| 0   |c1,c2| 4
| 1   |c3   | 8

Chunk0, size=1024B
|I|ent| c1   | c2   |
|0| 0 | data | data |
|1| 6 | data | data |
|2| 2 | data | data |
|3| 3 | data | data |


Chunk1, size=1024B
|I|ent| c3   |
|0| 4 | data |
|1| 5 | data |
|2| 1 | data |
|3| 7 | data |
|4| 8 | data |
|5| 9 | data |
|6| 10| data |
|7| 11| data |











*/










#define CSC_COMPONENT_MAX 64
#define CSC_ECS_UNDEFINED UINT32_MAX
#define CSC_ECS_CHUNK_SIZE 1024

struct csc_ecs
{
	uint32_t chunk_capacity;
	uint32_t chunk_count;
	void ** chunk;
	uint64_t * chunk_masks;
	uint32_t * chunk_entity_counts;
	uint32_t component_sizes[CSC_COMPONENT_MAX];
	void ** component;//Points to memories in chunk
};

struct csc_ecs_compinfo
{
	uint32_t sizes[CSC_COMPONENT_MAX];
	const char * names[CSC_COMPONENT_MAX];
};



void csc_ecs_init (struct csc_ecs * ecs)
{
	ecs->chunk = calloc (ecs->chunk_capacity, sizeof (void*));
	ecs->chunk_entity_counts = calloc (ecs->chunk_capacity, sizeof (uint32_t));
	ecs->chunk_masks = calloc (ecs->chunk_capacity, sizeof (uint64_t));
	ecs->component = calloc (ecs->chunk_capacity * CSC_COMPONENT_MAX, sizeof (void*));
	for (uint32_t i = 0; i < ecs->chunk_capacity; ++i)
	{
		ecs->chunk[i] = calloc (CSC_ECS_CHUNK_SIZE, sizeof (uint8_t));
	}
}



void csc_mask_unravel_u64 (uint64_t mask, uint64_t set[64])
{
	set[0] = mask & 0x0000000000000001;
	set[1] = mask & 0x0000000000000002;
	set[2] = mask & 0x0000000000000004;
	set[3] = mask & 0x0000000000000008;
	set[4] = mask & 0x0000000000000010;
	set[5] = mask & 0x0000000000000020;
	//TODO: etc...
}


void csc_ecs_add_chunk (struct csc_ecs * ecs, uint32_t sizes[CSC_COMPONENT_MAX], uint8_t n)
{
	uint32_t chunk_index = ecs->chunk_count;
	uint8_t * chunk = ecs->chunk[chunk_index];
	//https://en.wikipedia.org/wiki/Hamming_weight
	//https://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
	//int n = __builtin_popcountll (mask);

	uint32_t totalsize = 0;
	for (uint8_t i = 0; i < n; ++i)
	{
		totalsize += sizes[i];
	}
	uint32_t entity_count = CSC_ECS_CHUNK_SIZE / totalsize;
	ecs->chunk_entity_counts[chunk_index] = entity_count;
	for (uint8_t i = 0; i < n; ++i)
	{
		//Map memory:
		//Create shortcut for component data access:
		//A: size=1
		//B: size=2
		//C: size=3
		//n = 2
		//A,A BB,BB CCC,CCC,CCC
		ecs->component[chunk_index * CSC_COMPONENT_MAX + i] = chunk + sizes[i] * entity_count;
	}
	ecs->chunk_count++;

}










