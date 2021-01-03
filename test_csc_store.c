#include "csc_debug.h"
#include "csc_dlist.h"
#include "csc_store.h"



void test1()
{
	struct csc_store_u32 store;
	store.cap = 3;
	csc_store_u32_init (&store);

	printf ("Adding too many should return -1\n");
	csc_store_u32_add (&store, 5);
	csc_store_u32_add (&store, 6);
	csc_store_u32_add (&store, 7);
	ASSERT (csc_store_u32_add (&store, 8) == -1);

	printf ("Removing too many should return -1\n");
	uint32_t value = 0;
	csc_store_u32_remove (&store, &value);
	csc_store_u32_remove (&store, &value);
	csc_store_u32_remove (&store, &value);
	ASSERT (csc_store_u32_remove (&store, &value) == -1);
}



int main (int argc, char * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	test1();

	struct csc_store_u32 store;
	store.cap = 3;
	csc_store_u32_init (&store);
	csc_store_u32_add (&store, 5);
	csc_store_u32_add (&store, 6);
	csc_store_u32_add (&store, 7);
	csc_store_u32_add (&store, 8);
	uint32_t value = 0;
	csc_store_u32_remove (&store, &value);
	printf ("value %i\n", (int)value);
	csc_store_u32_remove (&store, &value);
	printf ("value %i\n", (int)value);
	csc_store_u32_remove (&store, &value);
	printf ("value %i\n", (int)value);
	csc_store_u32_remove (&store, &value);
	printf ("value %i\n", (int)value);


	//csc_store_u32_remove (&store);
	//csc_store_u32_add (&store, 6);
	//csc_store_u32_remove (&store);

	
	return EXIT_SUCCESS;
}
