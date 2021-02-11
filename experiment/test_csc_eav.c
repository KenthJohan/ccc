#include "csc_debug.h"
#include "csc_dlist.h"
#include "csc_eav.h"
#include "csc_basic.h"



#define COMP_POS 0
#define COMP_VEL 1
#define COMP_MASS 2


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);
	csc_crossos_enable_ansi_color();


	struct csc_eav eav;
	eav.attributes.capacity = 10;
	eav.entities.capacity = 10;
	eav.sparse.capacity = 10;
	csc_eav_init (&eav);

	uint32_t e1 = csc_eav_entities_gen (&eav.entities);
	uint32_t e2 = csc_eav_entities_gen (&eav.entities);
	uint32_t e3 = csc_eav_entities_gen (&eav.entities);
	uint32_t e4 = csc_eav_entities_gen (&eav.entities);
	float x[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	csc_eav_set (&eav, e1, COMP_POS, x);
	csc_eav_set (&eav, e2, COMP_POS, x);
	csc_eav_set (&eav, e3, COMP_POS, x);
	csc_eav_set (&eav, e4, COMP_POS, x);


	
	return EXIT_SUCCESS;
}
