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
	float * pos = ecs->component [CSC_COMPONENT_MAX * chunk_index + COMP_POS];
	float * vel = ecs->component [CSC_COMPONENT_MAX * chunk_index + COMP_VEL];
	uint32_t n = ecs->chunk_entity_counts[chunk_index];
	for (uint32_t i = 0; i < n; ++i)
	{
		pos[0] += vel[0];
		pos[1] += vel[1];
		pos[2] += vel[2];
		pos += 4;
		vel += 4;
	}
}

void system_draw (struct csc_ecs * ecs, uint32_t chunk_index)
{
	float * pos = ecs->component [CSC_COMPONENT_MAX * chunk_index + COMP_POS];
	uint32_t n = ecs->chunk_entity_counts[chunk_index];
	for (uint32_t i = 0; i < n; ++i)
	{
		pos += 4;
	}
}

void system_overall (struct csc_ecs * ecs)
{
	for (uint32_t chunk_index = 0; chunk_index < ecs->chunk_count; ++chunk_index)
	{
		if (ecs->chunk_masks[chunk_index] & (COMP_POS_BIT|COMP_VEL_BIT|COMP_MASS_BIT))
		{
			system_move (ecs, chunk_index);
		}
		if (ecs->chunk_masks[chunk_index] & (COMP_POS_BIT))
		{
			system_draw (ecs, chunk_index);
		}
	}
}




int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	struct csc_ecs_compinfo compinfo;
	compinfo.sizes[COMP_POS] = sizeof (float) * 2;
	compinfo.names[COMP_POS] = "Position";
	compinfo.sizes[COMP_VEL] = sizeof (float) * 2;
	compinfo.names[COMP_VEL] = "Velocity";
	compinfo.sizes[COMP_MASS] = sizeof (float) * 1;
	compinfo.names[COMP_MASS] = "Mass";

	struct csc_ecs ecs;

	{
		uint32_t sizes[3] = {compinfo.sizes[COMP_POS], compinfo.sizes[COMP_VEL], compinfo.sizes[COMP_MASS]};
		csc_ecs_add_chunk (&ecs, sizes, 3);
	}
	{
		uint32_t sizes[1] = {compinfo.sizes[COMP_POS]};
		csc_ecs_add_chunk (&ecs, sizes, 1);
	}

	
	return EXIT_SUCCESS;
}
