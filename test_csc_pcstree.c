#include "csc_debug.h"
#include "csc_pcstree.h"

typedef void (*csc_pcstree_print_traverse_cb)(struct csc_pcstree const * node, void *ptr);


/**
 * @brief Print pretty left-child right-sibling binary tree.
 * @param node Traverse the tree from this node downwards.
 * @param ptr User pointer that is passed on to the callback function \p cb.
 * @param cb A callback function that is called for each node in the tree.
 */
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


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);


	struct csc_pcstree * root = csc_pcstree_malloc();
	root->ptr = "root";
	ASSERT (root->child != NULL);
	struct csc_pcstree * c1 = csc_pcstree_malloc();
	c1->ptr = "root/c1";
	csc_pcstree_addchild (root, c1);
	ASSERT (root->next == root);
	ASSERT (root->prev == root);
	ASSERT (root->child->next == c1);
	ASSERT (c1->parent == root);
	struct csc_pcstree * s1 = csc_pcstree_malloc();
	s1->ptr = "s1";
	csc_pcstree_addsibling (root, s1);
	ASSERT (root->next == s1);
	ASSERT (root->prev == s1);
	ASSERT (root->child->next == c1);
	ASSERT (c1->parent == root);
	struct csc_pcstree * c2 = csc_pcstree_malloc();
	c2->ptr = "s1/c2";
	csc_pcstree_addchild (s1, c2);
	ASSERT (s1->prev == root);
	ASSERT (s1->next == root);
	ASSERT (s1->child->next == c2);
	ASSERT (c2->parent == s1);


	struct csc_pcstree * c4 = csc_pcstree_malloc();
	c4->ptr = "s1/c4";
	csc_pcstree_addsibling (c2, c4);

	struct csc_pcstree * c5 = csc_pcstree_malloc();
	c5->ptr = "s1/c2/c5";
	csc_pcstree_addchild (c2, c5);

	/*
	+-root
	| +-c1
	+-s1
	  +-c2
		+-c5
	  +-c4
	*/


	struct csc_pcstree * c3 = csc_pcstree_malloc();
	s1->child->ptr = "s1_child";
	c2->child->ptr = "c2_child";
	c3->child->ptr = "c3_child";
	c2->ptr = "s1/c3/c2";
	c3->ptr = "s1/c3";
	csc_pcstree_addparent (c2, c3);


	ASSERT (root->child->next == c1);
	ASSERT (root->next == s1);
	ASSERT (root->prev == s1);
	ASSERT (s1->prev == root);
	ASSERT (s1->next == root);
	ASSERT (s1->child->next == c3);
	ASSERT (c3->parent == s1);
	ASSERT (c3->next == c4);
	ASSERT (c3->child->next == c2);
	ASSERT (c2->parent == c3);
	ASSERT (c2->child->next == c5);
	ASSERT (c5->parent == c2);
	ASSERT (c2->next == c2);

	/*
	+-root
	| +-c1
	+-s1
	  +-c3
		+-c2
	  +-c4
	*/

	return EXIT_SUCCESS;
}
