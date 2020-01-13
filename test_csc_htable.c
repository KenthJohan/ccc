#include "csc_debug.h"
#include "csc_htable.h"



int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	struct csc_htable t;
	t.size = 4;
	csc_htable_init (&t);

	csc_htable_set_ptr (&t, "hej", "A");
	csc_htable_set_ptr (&t, "hej1112", "B");
	csc_htable_set_ptr (&t, "hej1", "B");
	csc_htable_set_imax (&t, "num1", 1);
	csc_htable_set_imax (&t, "num2", 2);
	csc_htable_set_imax (&t, "num3", 3);
	csc_htable_set_imax (&t, "num4", 4);

	struct csc_htable_entry * e;
	e = csc_htable_find (&t, "hej");
	printf ("%s %s\n", e->key, e->pval);
	e = csc_htable_find (&t, "hej1112");
	printf ("%s %s\n", e->key, e->pval);
	e = csc_htable_find (&t, "hej1");
	printf ("%s %s\n", e->key, e->pval);
	e = csc_htable_find (&t, "num1");
	printf ("%s %ju\n", e->key, e->ival);
	e = csc_htable_find (&t, "num2");
	printf ("%s %ju\n", e->key, e->ival);
	e = csc_htable_find (&t, "num3");
	printf ("%s %ju\n", e->key, e->ival);
	e = csc_htable_find (&t, "num4");
	printf ("%s %ju\n", e->key, e->ival);

	return EXIT_SUCCESS;
}
