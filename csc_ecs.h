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
	uint32_t size[CSC_COMPONENT_MAX];
	char const * name[CSC_COMPONENT_MAX];
};

struct csc_ecs_chunks
{
	uint32_t capacity;
	uint32_t count;
	uint8_t * memory;
	uint64_t * component_flags;
	uint32_t * entity_capacity;
	uint32_t * entity_count;
	uint32_t * component_sparse_offsets;
	uint32_t * component_totalsizes;
};

struct csc_ecs_entities
{
	uint32_t capacity;
	uint32_t count;
	uint32_t * chunk_i1; //Chunk index
	uint32_t * chunk_i2; //Index inside the chunk
};

struct csc_ecs
{
	struct csc_ecs_chunks chunks;
	struct csc_ecs_components components;
	struct csc_ecs_entities entities;
};


void csc_ecs_init (struct csc_ecs * ecs)
{
	ASSERT_NOTNULL (ecs);
	ecs->chunks.memory = calloc (ecs->chunks.capacity, CSC_ECS_CHUNK_SIZE);
	ecs->chunks.entity_capacity = calloc (ecs->chunks.capacity, sizeof (uint32_t));
	ecs->chunks.entity_count = calloc (ecs->chunks.capacity, sizeof (uint32_t));
	ecs->chunks.component_sparse_offsets = calloc (ecs->chunks.capacity * CSC_COMPONENT_MAX, sizeof (uint32_t));
	ecs->chunks.component_totalsizes = calloc (ecs->chunks.capacity, sizeof (uint32_t));
	ecs->chunks.component_flags = calloc (ecs->chunks.capacity, sizeof (uint64_t));

	ecs->entities.chunk_i1 = calloc (ecs->entities.capacity, sizeof (uint32_t));
	ecs->entities.chunk_i2 = calloc (ecs->entities.capacity, sizeof (uint32_t));

	//TODO: Add error handling:
	ASSERT_NOTNULL (ecs->chunks.memory);
	ASSERT_NOTNULL (ecs->chunks.entity_capacity);
	ASSERT_NOTNULL (ecs->chunks.entity_count);
	ASSERT_NOTNULL (ecs->chunks.component_sparse_offsets);
	ASSERT_NOTNULL (ecs->chunks.component_totalsizes);
	ASSERT_NOTNULL (ecs->chunks.component_flags);
	ASSERT_NOTNULL (ecs->entities.chunk_i1);
	ASSERT_NOTNULL (ecs->entities.chunk_i2);

	for (uint32_t i = 0; i < ecs->chunks.capacity * CSC_COMPONENT_MAX; ++i)
	{
		ecs->chunks.component_sparse_offsets[i] = CSC_ECS_UNDEFINED;
	}
	for (uint32_t i = 0; i < ecs->entities.capacity; ++i)
	{
		ecs->entities.chunk_i1[i] = CSC_ECS_UNDEFINED;
		ecs->entities.chunk_i2[i] = CSC_ECS_UNDEFINED;
	}
}


void csc_ecs_realloc (struct csc_ecs * ecs, uint32_t capacity)
{
	ASSERT_NOTNULL (ecs);
	ASSERTF (capacity >= ecs->chunks.capacity, "realloc will lose data if capacity is less than before");
	void *                      memory = realloc (ecs->chunks.memory, capacity * sizeof (void*));
	uint32_t *         entity_capacity = realloc (ecs->chunks.entity_capacity, ecs->chunks.capacity * sizeof (uint32_t));
	uint32_t * component_sparse_offset = realloc (ecs->chunks.component_sparse_offsets, ecs->chunks.capacity * CSC_COMPONENT_MAX * sizeof (uint32_t));
	uint32_t *     component_totalsize = realloc (ecs->chunks.component_totalsizes, ecs->chunks.capacity * sizeof (uint32_t));
	uint64_t *         component_flags = realloc (ecs->chunks.component_flags, ecs->chunks.capacity *sizeof (uint64_t));

	//TODO: Add error handling:
	ASSERT_NOTNULL (memory);
	ASSERT_NOTNULL (entity_capacity);
	ASSERT_NOTNULL (component_sparse_offset);
	ASSERT_NOTNULL (component_totalsize);
	ASSERT_NOTNULL (component_flags);

	//If all went well:
	ecs->chunks.capacity = capacity;
	ecs->chunks.memory = memory;
	ecs->chunks.entity_capacity = entity_capacity;
	ecs->chunks.component_sparse_offsets = component_sparse_offset;
	ecs->chunks.component_totalsizes = component_totalsize;
	ecs->chunks.component_flags = component_flags;
}


void * csc_ecs_data_chunk (struct csc_ecs * ecs, uint32_t chunk_index, uint32_t component_index)
{
	ASSERT_NOTNULL (ecs);
	ASSERT_NOTNULL (ecs->chunks.memory);
	ASSERT_NOTNULL (ecs->chunks.component_sparse_offsets);
	ASSERT (chunk_index < ecs->chunks.count);
	ASSERT (chunk_index < ecs->chunks.capacity);
	ASSERT (component_index < CSC_COMPONENT_MAX);
	uint32_t offset = ecs->chunks.component_sparse_offsets[CSC_COMPONENT_MAX * chunk_index + component_index];
	ASSERT (offset != CSC_ECS_UNDEFINED);
	uint8_t * memory = ecs->chunks.memory + offset;
	ASSERTF (memory < (ecs->chunks.memory + CSC_ECS_CHUNK_SIZE * ecs->chunks.count), "Memory pointer is outside allocated space");
	return memory;
}


void * csc_ecs_data_entity (struct csc_ecs * ecs, uint32_t entity_index, uint32_t component_index)
{
	ASSERT_NOTNULL (ecs);
	ASSERT_NOTNULL (ecs->chunks.component_sparse_offsets);
	ASSERT_NOTNULL (ecs->components.size);
	ASSERT_NOTNULL (ecs->entities.chunk_i1);
	ASSERT_NOTNULL (ecs->entities.chunk_i2);
	ASSERT (entity_index < ecs->entities.count);
	ASSERT (entity_index < ecs->entities.capacity);
	ASSERT (component_index < CSC_COMPONENT_MAX);
	uint32_t chunk_i1 = ecs->entities.chunk_i1[entity_index];
	uint32_t chunk_i2 = ecs->entities.chunk_i2[entity_index];
	ASSERT (chunk_i1 != CSC_ECS_UNDEFINED);
	ASSERT (chunk_i2 != CSC_ECS_UNDEFINED);
	uint32_t offset = ecs->chunks.component_sparse_offsets[CSC_COMPONENT_MAX * chunk_i1 + component_index];
	ASSERT (offset != CSC_ECS_UNDEFINED);
	uint32_t size = ecs->components.size[component_index];
	ASSERT (size != CSC_ECS_UNDEFINED);
	uint8_t * memory = ecs->chunks.memory + offset + (size * chunk_i2);
	ASSERTF (memory < (ecs->chunks.memory + CSC_ECS_CHUNK_SIZE * ecs->chunks.count), "Memory pointer is outside allocated space");
	return memory;
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
	ASSERT_NOTNULL (ecs);
	ASSERT_NOTNULL (comps);
	ASSERT (n < CSC_COMPONENT_MAX);

	uint32_t chunk_index = ecs->chunks.count;

	ASSERT_NOTNULL (ecs->chunks.component_sparse_offsets);
	ASSERT_NOTNULL (ecs->chunks.component_totalsizes);
	ASSERT_NOTNULL (ecs->components.size);
	ASSERT_NOTNULL (ecs->chunks.entity_capacity);
	ASSERT (chunk_index < ecs->chunks.capacity);

	//Calculate total size of components:
	uint32_t totalsize = 0;
	for (uint8_t i = 0; i < n; ++i)
	{
		uint32_t c = comps[i];
		ASSERT (c < CSC_COMPONENT_MAX);
		totalsize += ecs->components.size[c];
		ecs->chunks.component_flags[chunk_index] |= (1 << c);
	}
	 ecs->chunks.component_totalsizes[chunk_index] = totalsize;

	//Calculate maximum entities this chunk can handle:
	uint32_t entity_count = CSC_ECS_CHUNK_SIZE / totalsize;
	ecs->chunks.entity_capacity[chunk_index] = entity_count;

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
		ecs->chunks.component_sparse_offsets[CSC_COMPONENT_MAX * chunk_index + c] = (CSC_ECS_CHUNK_SIZE * chunk_index) + size;
		size += ecs->components.size[c] * entity_count;
	}

	//TODO: Check a bunch stuff if everything is okey:
	//uint8_t * memory = ecs->chunks.memory + (CSC_ECS_CHUNK_SIZE * chunk_index);
	//ASSERT (memory);

	//If all went well we can incrament number of chunks:
	ecs->chunks.count++;

	//If number of chunks has reached critical capacity then we need to allocate more to be safe:
	if (ecs->chunks.count >= ecs->chunks.capacity)
	{
		//Reduce number of future realloc by allating more memory than we need:
		uint32_t capacity = csc_round_up_pow2 (ecs->chunks.capacity);
		csc_ecs_realloc (ecs, capacity);
	}

	ASSERT (ecs->chunks.count < ecs->chunks.capacity);
	ASSERT (chunk_index < ecs->chunks.count);
	return chunk_index;
}


//TODO: Create effecient entity generator:
//Generating a entity should only create or reuse a unique index:
uint32_t csc_ecs_gen_entity_inactive (struct csc_ecs * ecs)
{
	ASSERT_NOTNULL (ecs);
	//TODO: Recycle unused entities.
	uint32_t entity_index = ecs->entities.count;
	ecs->entities.count++;
	return entity_index;
};


//Internal function
//Use this:
//* if you know which chunk the entity should be in.
//* if entity has already been generated.
void csc_ecs__add_entity (struct csc_ecs * ecs, uint32_t entity_index, uint32_t chunk_index)
{
	ASSERT_NOTNULL (ecs);
	ASSERT_NOTNULL (ecs->entities.chunk_i1);
	ASSERT_NOTNULL (ecs->entities.chunk_i2);
	ASSERT_NOTNULL (ecs->chunks.entity_count);
	ASSERT_NOTNULL (ecs->chunks.entity_capacity);
	ASSERT (chunk_index != CSC_ECS_UNDEFINED);
	ASSERT (ecs->chunks.entity_count[chunk_index] < ecs->chunks.entity_capacity[chunk_index]);
	ecs->entities.chunk_i1[entity_index] = chunk_index;
	ecs->entities.chunk_i2[entity_index] = ecs->chunks.entity_count[chunk_index];
	ecs->chunks.entity_count[chunk_index]++;
	ASSERT (ecs->chunks.entity_count[chunk_index] <= ecs->chunks.entity_capacity[chunk_index]);
}


void csc_ecs_gen_entity (struct csc_ecs * ecs, uint64_t mask, uint32_t * out_chunk_index, uint32_t * out_entity_index)
{
	ASSERT_NOTNULL (ecs);
	ASSERT_NOTNULL(ecs->chunks.component_flags);
	ASSERT_NOTNULL(ecs->chunks.entity_count);
	ASSERT_NOTNULL(ecs->chunks.entity_capacity);
	ASSERT (ecs->entities.count < ecs->entities.capacity);

	//Find the chunk which has the same components as entity:
	uint32_t chunk_index = CSC_ECS_UNDEFINED;
	for (uint32_t i = 0; i < ecs->chunks.count; ++i)
	{
		//Check if chunk is full of entities:
		if (ecs->chunks.entity_count[i] >= ecs->chunks.entity_capacity[i])
		{
			continue;
		}

		//https://www.chessprogramming.org/General_Setwise_Operations
		//Equivalence: The chunk has exactly the same components as entity:
		if (ecs->chunks.component_flags[i] == mask)
		{
			//TODO: This might increases number of chunks significantly!
			chunk_index = i;
		}
		//Subset: Entity components is a subset of chunk components:
		if ((ecs->chunks.component_flags[i] & mask) == mask)
		{
			//TODO: This will activate wrong systems! There could be a workaround.
			//Need to keep track of induvidial entities components.
		}
	}

	//TODO: Create chunk if not found:

	//Entity derives this chunk components:
	uint32_t entity_index = csc_ecs_gen_entity_inactive (ecs);
	csc_ecs__add_entity (ecs, entity_index, chunk_index);


	if (out_chunk_index)
	{
		(*out_chunk_index) = chunk_index;
	}

	if (out_entity_index)
	{
		(*out_entity_index) = entity_index;
	}
}







