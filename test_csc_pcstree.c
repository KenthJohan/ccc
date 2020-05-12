#include "csc_debug.h"
#include "csc_pcstree.h"

typedef void (*csc_pcstree_print_traverse_cb)(struct csc_pcstree const * node, void *ptr);


/**
 * @brief Print pretty left-child right-sibling binary tree.
 * @param node Traverse the tree from this node downwards.
 * @param ptr User pointer that is passed on to the callback function \p cb.
 * @param cb A callback function that is called for each node in the tree.
 */
/*
void csc_pcstree_print_traverse (struct csc_pcstree const * node, void * ptr, csc_pcstree_print_traverse_cb cb)
{
	char vertical[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u2502" TCOL_RST;
	char cross[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u251C" TCOL_RST;
	char corner[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u2514" TCOL_RST;
	char dash[] = TCOL(TCOL_NORMAL,TCOL_GREEN,TCOL_DEFAULT) "\u2500" TCOL_RST;
	int depth = 0;
	//int leaf = 0;
	uint32_t indent = 0;
	while (node)
	{
		for (int i = 0; i < depth; i ++)
		{
			if (indent & (1 << i))
			{
				printf ("%s", vertical);
			}
			else
			{
				putc (' ', stdout);
			}
			putc (' ', stdout);
			putc (' ', stdout);
		}
		if (node->next)
		{
			printf ("%s", cross);
			indent |= (1 << depth);
		}
		if (node->next == NULL)
		{
			printf ("%s", corner);
			indent &= ~(1 << depth);
		}
		printf ("%s", dash);
		cb (node, ptr);
		node = csc_pcstree_traverse (node, &depth);
	}
}


void traverse_cb (struct csc_pcstree const * nodepos, void * ptr)
{
	ASSERT_PARAM_NOTNULL (nodepos);
	printf (" %s\n", nodepos->ptr);
}
*/

enum testnode
{
	TESTNODE_A,
	TESTNODE_B,
	TESTNODE_C,
	TESTNODE_D,
	TESTNODE_E,
	TESTNODE_F,
	TESTNODE_G,
	TESTNODE_H,
	TESTNODE_I,
	TESTNODE_J,
	TESTNODE_K,
	TESTNODE_L,
	TESTNODE_M,
	TESTNODE_N
};


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);


	struct csc_pcstree node [20];
	int x [20] = {INT_MAX};
	node [TESTNODE_A].name = "A";
	node [TESTNODE_B].name = "B";
	node [TESTNODE_C].name = "C";
	node [TESTNODE_D].name = "D";
	node [TESTNODE_E].name = "E";
	node [TESTNODE_F].name = "F";
	node [TESTNODE_G].name = "G";
	node [TESTNODE_H].name = "H";
	node [TESTNODE_I].name = "I";
	node [TESTNODE_J].name = "J";
	node [TESTNODE_K].name = "K";
	node [TESTNODE_L].name = "L";
	node [TESTNODE_M].name = "M";
	node [TESTNODE_N].name = "N";
	csc_pcstree_init (node + TESTNODE_A);
	csc_pcstree_init (node + TESTNODE_B);
	csc_pcstree_init (node + TESTNODE_C);
	csc_pcstree_init (node + TESTNODE_D);
	csc_pcstree_init (node + TESTNODE_E);
	csc_pcstree_init (node + TESTNODE_F);
	csc_pcstree_init (node + TESTNODE_G);
	csc_pcstree_init (node + TESTNODE_H);
	csc_pcstree_init (node + TESTNODE_I);
	csc_pcstree_init (node + TESTNODE_J);
	csc_pcstree_init (node + TESTNODE_K);
	csc_pcstree_init (node + TESTNODE_L);
	csc_pcstree_init (node + TESTNODE_M);
	csc_pcstree_init (node + TESTNODE_N);


	csc_pcstree_add (node+TESTNODE_A, node+TESTNODE_B);
	csc_pcstree_add (node+TESTNODE_B, node+TESTNODE_C);




	return EXIT_SUCCESS;
}
