#include <stdio.h>
#include <stdint.h>
#include "csc_crossos.h"
#include "csc_assert.h"
#include "csc_dlist.h"


#define NODE_COUNT 10





void test1()
{
	struct csc_dlist head1;
	struct csc_dlist head2;
	struct csc_dlist nodes [4];
	csc_dlist_init (&head1);
	csc_dlist_init (&head2);

	csc_dlist_addv (&head1, nodes, 4);
	ASSERT (csc_dlist_count (&head1) == 4);
	ASSERT (csc_dlist_count (&head2) == 0);

	csc_dlist_move_head (&head2, nodes + 0);
	ASSERT (csc_dlist_count (&head1) == 3);
	ASSERT (csc_dlist_count (&head2) == 1);

	//This should have no effect:
	csc_dlist_move_head (&head2, nodes + 0);
	csc_dlist_move_head (&head2, nodes + 0);
	csc_dlist_move_head (&head2, nodes + 0);
	csc_dlist_move_head (&head2, nodes + 0);
	ASSERT (csc_dlist_count (&head1) == 3);
	ASSERT (csc_dlist_count (&head2) == 1);


	csc_dlist_move_head (&head2, nodes + 1);
	ASSERT (csc_dlist_count (&head1) == 2);
	ASSERT (csc_dlist_count (&head2) == 2);

	csc_dlist_move_head (&head2, nodes + 2);
	ASSERT (csc_dlist_count (&head1) == 1);
	ASSERT (csc_dlist_count (&head2) == 3);

	csc_dlist_move_head (&head2, nodes + 3);
	ASSERT (csc_dlist_count (&head1) == 0);
	ASSERT (csc_dlist_count (&head2) == 4);
}






int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);
	csc_crossos_enable_ansi_color();
	test1();
	/*
	struct csc_dlist head;
	struct csc_dlist nodes [NODE_COUNT];
	csc_dlist_init (&head);
	csc_dlist_addv (&head, nodes, NODE_COUNT);
	ASSERT (csc_dlist_count (&head) == NODE_COUNT);

	{
		struct csc_dlist * p;
		uint32_t n;
		for (p = head.next, n = 0; p != &head; p = p->next, ++n)
		{
			ASSERT (n < NODE_COUNT);
			ASSERT (p);
		}
	}
	*/


	/*
	ASSERT (csc_dlist_count (node) == 0);
	csc_dlist_add_head (node, node+1);
	ASSERT (csc_dlist_count (node) == 1);
	csc_dlist_add_head (node, node+2);
	ASSERT (csc_dlist_count (node) == 2);
	csc_dlist_del (node+1);
	ASSERT (csc_dlist_count (node) == 1);
	csc_dlist_del (node+2);
	ASSERT (csc_dlist_count (node) == 0);
	*/



	
	return EXIT_SUCCESS;
}
