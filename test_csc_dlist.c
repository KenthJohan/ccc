#include "csc_debug.h"
#include "csc_dlist.h"



int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	struct csc_dlist node [10];
	csc_dlist_init (node);
	ASSERT (csc_dlist_count (node) == 0);
	csc_dlist_add_head (node, node+1);
	ASSERT (csc_dlist_count (node) == 1);
	csc_dlist_add_head (node, node+2);
	ASSERT (csc_dlist_count (node) == 2);
	
	return EXIT_SUCCESS;
}
