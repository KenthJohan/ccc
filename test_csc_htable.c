#include "csc_debug.h"
#include "csc_htable.h"
#include "csc_dlist.h"
#include "csc_basic.h"



struct test_entry
{
	struct csc_htable_entry entry;
	char * value;
};


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);


	struct csc_htable t;
	t.size = 4;
	csc_htable_init (&t);



	{
		struct test_entry * e;
		e = malloc (sizeof (struct test_entry));
		e->entry.key = "Hello";
		e->value = "Hello";
		csc_htable_set (&t, &e->entry);
		ASSERT (csc_dlist_count (&t.list_replace) == 0);
	}


	{
		struct test_entry * e;
		e = malloc (sizeof (struct test_entry));
		e->entry.key = "Hello_C";
		e->value = "Hello_C";
		csc_htable_set (&t, &e->entry);
		ASSERT (csc_dlist_count (&t.list_replace) == 0);
	}


	{
		struct test_entry * e;
		e = malloc (sizeof (struct test_entry));
		e->entry.key = "Hello";
		e->value = "Hello2";
		csc_htable_set (&t, &e->entry);
		ASSERT (csc_dlist_count (&t.list_replace) == 1);
	}


	{
		struct test_entry * e;
		e = malloc (sizeof (struct test_entry));
		e->entry.key = "Hello";
		e->value = "Hello3";
		csc_htable_set (&t, &e->entry);
		ASSERT (csc_dlist_count (&t.list_replace) == 2);
	}

	{
		struct test_entry * e;
		e = malloc (sizeof (struct test_entry));
		e->entry.key = "Hello_C";
		e->value = "Hello_C1";
		csc_htable_set (&t, &e->entry);
		ASSERT (csc_dlist_count (&t.list_replace) == 3);
	}


	TRACEF ("list_replace %i", csc_dlist_count (&t.list_replace));

	{
		for (struct csc_dlist * p = t.list_replace.next; p != &t.list_replace; p = p->next)
		{
			struct csc_htable_entry * e = container_of (p, struct csc_htable_entry, list);
			struct test_entry * g = container_of (e, struct test_entry, entry);
			printf ("%s\n", g->value);
		}
	}



	/*
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
	*/




	return EXIT_SUCCESS;
}
