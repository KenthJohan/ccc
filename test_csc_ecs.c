#include "csc_debug.h"
#include "csc_dlist.h"
#include "csc_ecs.h"



#define COMP_POS 0
#define COMP_VEL 1
#define COMP_MASS 2
#define COMP_POS_BIT 0x01
#define COMP_VEL_BIT 0x02
#define COMP_MASS_BIT 0x04


void system_move (struct csc_ecs * ecs, uint32_t chunk_index)
{
	float * pos = csc_ecs_get_data (ecs, chunk_index, COMP_POS);
	float * vel = csc_ecs_get_data (ecs, chunk_index, COMP_VEL);
	uint32_t n = ecs->chunks.entity_counts[chunk_index];
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
	uint32_t n = ecs->chunks.entity_counts[chunk_index];
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

	csc_ecs_test1();

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
