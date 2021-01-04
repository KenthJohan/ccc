#include "csc_debug.h"
#include "csc_dlist.h"
#include "csc_ecs.h"



#define COMP_POS 0
#define COMP_VEL 1
#define COMP_MASS 2
#define COMP_POS_BIT 0x01
#define COMP_VEL_BIT 0x02
#define COMP_MASS_BIT 0x04


void print_hr(){printf("============================================================\n");}


void test1()
{
	enum my_comp
	{
		COMP_A,
		COMP_B,
		COMP_C,
	};
	struct csc_ecs ecs = {0};
	ecs.chunks.capacity = 4;
	ecs.entities.capacity = 4;
	uint32_t * sizes = ecs.components.sizes; //Rename
	sizes[COMP_A] = sizeof (float) * 1;
	sizes[COMP_B] = sizeof (float) * 2;
	sizes[COMP_C] = sizeof (float) * 7;
	csc_ecs_init (&ecs);

	{
		printf ("csc_ecs_add_chunk (COMP_A, COMP_B, COMP_C):\n");
		enum my_comp comps[3] = {COMP_A, COMP_B, COMP_C};
		uint32_t chunk_index = csc_ecs_add_chunk (&ecs, comps, 3);
		uint32_t entity_count = ecs.chunks.entity_caps[chunk_index];
		uint32_t component_totalsize = ecs.chunks.component_totalsizes[chunk_index];
		printf ("         chunk_index: %u\n", chunk_index);
		printf ("               sizes: (%u, %u, %u)\n", sizes[comps[0]], sizes[comps[1]], sizes[comps[2]]);
		printf ("        entity count: %u\n", entity_count);
		printf (" component totalsize: %u\n", component_totalsize);
		printf ("     chunk totalsize: %u = %u * %u\n", component_totalsize * entity_count, component_totalsize, entity_count);
		printf ("allocated chunk size: %u\n", CSC_ECS_CHUNK_SIZE);
		ASSERT (component_totalsize == (sizes[COMP_A] + sizes[COMP_B] + sizes[COMP_C]));
		print_hr();
	}

	{
		printf ("csc_ecs_add_chunk (COMP_C, COMP_A):\n");
		enum my_comp comps[2] = {COMP_C, COMP_A};
		uint32_t chunk_index = csc_ecs_add_chunk (&ecs, comps, 2);
		uint32_t entity_count = ecs.chunks.entity_caps[chunk_index];
		uint32_t component_totalsize = ecs.chunks.component_totalsizes[chunk_index];
		printf ("         chunk_index: %u\n", chunk_index);
		printf ("               sizes: (%u, %u)\n", sizes[comps[0]], sizes[comps[1]]);
		printf ("        entity count: %u\n", entity_count);
		printf (" component totalsize: %u\n", component_totalsize);
		printf ("     chunk totalsize: %u = %u * %u\n", component_totalsize * entity_count, component_totalsize, entity_count);
		printf ("allocated chunk size: %u\n", CSC_ECS_CHUNK_SIZE);
		ASSERT (component_totalsize == (sizes[COMP_C] + sizes[COMP_A]));
		print_hr();
	}

	{
		printf ("csc_ecs_add_entity (COMP_A, COMP_B, COMP_C):\n");
		uint32_t chunk_index;
		uint32_t entity_index;
		csc_ecs_add_entity (&ecs, (1 << COMP_A) | (1 << COMP_B) | (1 << COMP_C), &chunk_index, &entity_index);
		printf ("         chunk_index: %u\n", chunk_index);
		printf ("          entity cap: %u\n", ecs.entities.capacity);
		printf ("        entity count: %u\n", ecs.entities.count);
		printf ("    chunk entity cap: %u\n", ecs.chunks.entity_caps[chunk_index]);
		printf ("  chunk entity count: %u\n", ecs.chunks.entity_count[chunk_index]);
		print_hr();
	}

	{
		printf ("csc_ecs_add_entity (COMP_C, COMP_A):\n");
		uint32_t chunk_index;
		uint32_t entity_index;
		csc_ecs_add_entity (&ecs, (1 << COMP_C) | (1 << COMP_A), &chunk_index, &entity_index);
		printf ("         chunk_index: %u\n", chunk_index);
		printf ("          entity cap: %u\n", ecs.entities.capacity);
		printf ("        entity count: %u\n", ecs.entities.count);
		printf ("    chunk entity cap: %u\n", ecs.chunks.entity_caps[chunk_index]);
		printf ("  chunk entity count: %u\n", ecs.chunks.entity_count[chunk_index]);
		print_hr();
	}

	{
		printf ("csc_ecs_add_entity (COMP_C, COMP_A):\n");
		uint32_t chunk_index;
		uint32_t entity_index;
		csc_ecs_add_entity (&ecs, (1 << COMP_C) | (1 << COMP_A), &chunk_index, &entity_index);
		printf ("         chunk_index: %u\n", chunk_index);
		printf ("          entity cap: %u\n", ecs.entities.capacity);
		printf ("        entity count: %u\n", ecs.entities.count);
		printf ("    chunk entity cap: %u\n", ecs.chunks.entity_caps[chunk_index]);
		printf ("  chunk entity count: %u\n", ecs.chunks.entity_count[chunk_index]);
		print_hr();
	}

}






void system_move (struct csc_ecs * ecs, uint32_t chunk_index)
{
	float * pos = csc_ecs_get_data (ecs, chunk_index, COMP_POS);
	float * vel = csc_ecs_get_data (ecs, chunk_index, COMP_VEL);
	uint32_t n = ecs->chunks.entity_caps[chunk_index];
	for (uint32_t i = 0; i < n; ++i)
	{
		pos[0] += vel[0];
		pos[1] += vel[1];
		pos[2] += vel[2];
		pos += 2;
		vel += 2;
	}
}

void system_draw (struct csc_ecs * ecs, uint32_t chunk_index)
{
	float * pos = csc_ecs_get_data (ecs, chunk_index, COMP_POS);
	uint32_t n = ecs->chunks.entity_caps[chunk_index];
	for (uint32_t i = 0; i < n; ++i)
	{
		pos += 4;
	}
}

void system_overall (struct csc_ecs * ecs)
{
	for (uint32_t chunk_index = 0; chunk_index < ecs->chunks.count; ++chunk_index)
	{
		uint64_t mask = ecs->chunks.masks[chunk_index];
		if ((mask & (COMP_POS_BIT|COMP_VEL_BIT|COMP_MASS_BIT)) == (COMP_POS_BIT|COMP_VEL_BIT|COMP_MASS_BIT))
		{
			system_move (ecs, chunk_index);
		}
		if ((mask & (COMP_POS_BIT)) == (COMP_POS_BIT))
		{
			system_draw (ecs, chunk_index);
		}
	}
}




int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);
	csc_crossos_enable_ansi_color();

	test1();

	/*
	struct csc_ecs ecs;
	ecs.components.sizes[COMP_POS] = sizeof (float) * 2;
	ecs.components.names[COMP_POS] = "Position";
	ecs.components.sizes[COMP_VEL] = sizeof (float) * 2;
	ecs.components.names[COMP_VEL] = "Velocity";
	ecs.components.sizes[COMP_MASS] = sizeof (float) * 1;
	ecs.components.names[COMP_MASS] = "Mass";

	{
		uint32_t comps[3] = {COMP_POS, COMP_VEL, COMP_MASS};
		csc_ecs_add_chunk (&ecs, comps, 3);
	}
	{
		uint32_t comps[1] = {COMP_POS};
		csc_ecs_add_chunk (&ecs, comps, 1);
	}
	*/

	
	return EXIT_SUCCESS;
}
