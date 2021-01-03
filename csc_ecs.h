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


struct csc_ecs_components
{
	uint32_t sizes[CSC_COMPONENT_MAX];
	const char * names[CSC_COMPONENT_MAX];
};

struct csc_ecs_chunks
{
	uint32_t capacity;
	uint32_t count;
	void ** memory;
	uint64_t * masks;
	uint32_t * entity_counts;
	uint32_t * component_sparse_offsets;
	uint32_t * component_totalsizes;
};



struct csc_ecs
{
	struct csc_ecs_chunks chunks;
	struct csc_ecs_components components;
};





void csc_ecs_init (struct csc_ecs * ecs)
{
	ecs->chunks.memory = calloc (ecs->chunks.capacity, sizeof (void*));
	ecs->chunks.entity_counts = calloc (ecs->chunks.capacity, sizeof (uint32_t));
	ecs->chunks.component_sparse_offsets = calloc (ecs->chunks.capacity * CSC_COMPONENT_MAX, sizeof (uint32_t));
	ecs->chunks.component_totalsizes = calloc (ecs->chunks.capacity, sizeof (uint32_t));
	ecs->chunks.masks = calloc (ecs->chunks.capacity, sizeof (uint64_t));

	ASSERT_NOTNULL (ecs->chunks.memory);
	ASSERT_NOTNULL (ecs->chunks.entity_counts);
	ASSERT_NOTNULL (ecs->chunks.component_sparse_offsets);
	ASSERT_NOTNULL (ecs->chunks.component_totalsizes);
	ASSERT_NOTNULL (ecs->chunks.masks);

	for (uint32_t i = 0; i < ecs->chunks.capacity; ++i)
	{
		ecs->chunks.memory[i] = calloc (CSC_ECS_CHUNK_SIZE, sizeof (uint8_t));
	}

	for (uint32_t i = 0; i < ecs->chunks.capacity * CSC_COMPONENT_MAX; ++i)
	{
		ecs->chunks.component_sparse_offsets[i] = CSC_ECS_UNDEFINED;
	}
}






void * csc_ecs_get_data (struct csc_ecs * ecs, uint32_t chunk_index, uint32_t component_index)
{
	uint8_t * chunk = ecs->chunks.memory[chunk_index];
	uint32_t offset = ecs->chunks.component_sparse_offsets[CSC_COMPONENT_MAX * chunk_index + component_index];
	return chunk + offset;
}


/*
Components A,BB,CC:
  A: size=1
 BB: size=2
CCC: size=3
entity_count = 3
total_size = (A+BB+CCC) * 3 = (AAA+BBB+BBB+CCC+CCC+CCC) = (3A+6B+9C) = 18 bytes
memory = AAABBBBBBCCCCCCCCC______
offset = 0  3     9        18
offset = (0,3,9)

|I|A|BB|CCC|___|
| |0| 3|  9| 18| <--- offsets
|0|A|BB|CCC|
|1|A|BB|CCC|
|2|A|BB|CCC|

component A   memory = A A A
component BB  memory = BB BB BB
component CCC memory = CCC CCC CCC

*/
uint32_t csc_ecs_add_chunk (struct csc_ecs * ecs, uint32_t comps[CSC_COMPONENT_MAX], uint8_t n)
{
	//Rename a bunch of variables:
	uint32_t     chunk_index = ecs->chunks.count;
	uint32_t *       offsets = ecs->chunks.component_sparse_offsets;
	uint32_t *    totalsizes = ecs->chunks.component_totalsizes;
	uint32_t *         sizes = ecs->components.sizes;
	uint32_t * entity_counts = ecs->chunks.entity_counts;

	ASSERT_NOTNULL (offsets);
	ASSERT_NOTNULL (totalsizes);
	ASSERT_NOTNULL (sizes);
	ASSERT_NOTNULL (entity_counts);
	ASSERT (chunk_index < ecs->chunks.capacity);

	//Calculate total size of components:
	uint32_t totalsize = 0;
	for (uint8_t i = 0; i < n; ++i)
	{
		uint32_t c = comps[i];
		totalsize += sizes[c];
	}
	totalsizes[chunk_index] = totalsize;

	//Calculate max entity count this chunk can handle:
	uint32_t entity_count = CSC_ECS_CHUNK_SIZE / totalsize;
	entity_counts[chunk_index] = entity_count;

	ASSERTF ((totalsize * entity_count) <= CSC_ECS_CHUNK_SIZE, "(%u %u = %u) < %u", totalsize, entity_count, totalsize * entity_count, CSC_ECS_CHUNK_SIZE);

	//Calculate offset for every component where memory starts:
	uint32_t size = 0;
	for (uint8_t i = 0; i < n; ++i)
	{
		//(comps) is array of component indices:
		uint32_t c = comps[i];
		ASSERT (c < CSC_COMPONENT_MAX);
		ASSERT (size < CSC_ECS_CHUNK_SIZE);
		//Create a sparse set for every offset:
		//The component index is limited to CSC_COMPONENT_MAX:
		offsets[CSC_COMPONENT_MAX * chunk_index + c] = size;
		size += sizes[c] * entity_count;
	}

	//TODO: Check a bunch stuff if everything is okey:
	uint8_t * chunk = ecs->chunks.memory[chunk_index];
	ASSERT (chunk);

	//If all went well we can incrament number of chunks:
	ecs->chunks.count++;


	return chunk_index;
}



void csc_ecs_test1()
{
	enum my_comp
	{
		COMP_A,
		COMP_B,
		COMP_C,
	};
	struct csc_ecs ecs = {0};
	ecs.chunks.capacity = 4;
	uint32_t * sizes = ecs.components.sizes;
	sizes[COMP_A] = sizeof (float) * 1;
	sizes[COMP_B] = sizeof (float) * 2;
	sizes[COMP_C] = sizeof (float) * 7;
	csc_ecs_init (&ecs);

	{
		enum my_comp comps[3] = {COMP_A, COMP_B, COMP_C};
		uint32_t chunk_index = csc_ecs_add_chunk (&ecs, comps, 3);
		uint32_t entity_count = ecs.chunks.entity_counts[chunk_index];
		uint32_t component_totalsize = ecs.chunks.component_totalsizes[chunk_index];
		printf ("               sizes: (%u, %u, %u)\n", sizes[comps[0]], sizes[comps[1]], sizes[comps[2]]);
		printf ("        entity count: %u\n", entity_count);
		printf (" component totalsize: %u\n", component_totalsize);
		printf ("     chunk totalsize: %u = %u * %u\n", component_totalsize * entity_count, component_totalsize, entity_count);
		printf ("allocated chunk size: %u\n", CSC_ECS_CHUNK_SIZE);
		ASSERT (component_totalsize == ((1+2+7)*sizeof(float)));
	}

	{
		enum my_comp comps[2] = {COMP_C, COMP_A};
		uint32_t chunk_index = csc_ecs_add_chunk (&ecs, comps, 2);
		uint32_t entity_count = ecs.chunks.entity_counts[chunk_index];
		uint32_t component_totalsize = ecs.chunks.component_totalsizes[chunk_index];
		printf ("               sizes: (%u, %u)\n", sizes[comps[0]], sizes[comps[1]]);
		printf ("        entity count: %u\n", entity_count);
		printf (" component totalsize: %u\n", component_totalsize);
		printf ("     chunk totalsize: %u = %u * %u\n", component_totalsize * entity_count, component_totalsize, entity_count);
		printf ("allocated chunk size: %u\n", CSC_ECS_CHUNK_SIZE);
		ASSERT (component_totalsize == ((7+1)*sizeof(float)));
	}

}








