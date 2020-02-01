#include "csc_debug.h"
#include "csc_htable.h"
#include "csc_dlist.h"
#include "csc_basic.h"


#define BUCKETS_COUNT 100
#define BUCKETS_NAME_COUNT 10
#define BUCKETS_HT_COUNT 4


struct buckets
{
	struct csc_dlist ht [BUCKETS_HT_COUNT];
	char * name;
	int * count;
	struct csc_dlist * node;
};

void buckets_set (struct buckets * b, unsigned i, char const * str, int count)
{
	char * name = b->name + BUCKETS_NAME_COUNT * i;
	struct csc_dlist * node = b->node + i;
	snprintf (name, BUCKETS_NAME_COUNT, "%s", str);
	b->count [i] = count;
	uint8_t hv = hash8_str (name, BUCKETS_HT_COUNT);
	ASSERT (node->next == NULL);
	ASSERT (node->prev == NULL);
	csc_dlist_add_head (b->ht + hv, node);
}

int buckets_find (struct buckets * b, char const * needle)
{
	uint8_t hv = hash8_str (needle, BUCKETS_HT_COUNT);
	struct csc_dlist * p = b->ht + hv;
	int i = csc_dlist_compare_str (p, b->node, needle, b->name, BUCKETS_NAME_COUNT, 0);
	return i;
}

int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	struct buckets b;
	csc_dlist_init_v (b.ht, BUCKETS_HT_COUNT);
	b.node = calloc (BUCKETS_COUNT, sizeof (struct csc_dlist));
	b.name = calloc (BUCKETS_COUNT * BUCKETS_NAME_COUNT, sizeof (char));
	b.count = calloc (BUCKETS_COUNT, sizeof (int));

	buckets_set (&b, 0, "Banana", 200);
	buckets_set (&b, 1, "Apples", 300);
	buckets_set (&b, 2, "Lemons", 500);
	buckets_set (&b, 3, "Oranges", 200);
	buckets_set (&b, 4, "Pears", 150);

	ASSERT (csc_dlist_empty_v (b.ht, BUCKETS_HT_COUNT) != 0);
	ASSERT (csc_dlist_nonempty_v (b.ht, BUCKETS_HT_COUNT) != 0);
	ASSERT (csc_dlist_empty_v (b.ht, BUCKETS_HT_COUNT) + csc_dlist_nonempty_v (b.ht, BUCKETS_HT_COUNT) == BUCKETS_HT_COUNT);

	ASSERT (buckets_find (&b, "Lemons") >= 0);
	ASSERT (strcmp (b.name + buckets_find (&b, "Lemons") * BUCKETS_NAME_COUNT, "Lemons") == 0);

	return EXIT_SUCCESS;
}
