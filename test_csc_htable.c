#include "csc_debug.h"
#include "csc_htable.h"
#include "csc_dlist.h"
#include "csc_basic.h"




int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);
	setbuf (stdout, NULL);

	struct csc_htablestr ht;
	ht.htsize = 4;
	ht.name_stride = 20;
	ht.items_max = 30;
	csc_htablestr_init (&ht);

	csc_htablestr_add (&ht, "Apple", NULL);
	csc_htablestr_add (&ht, "Banana", NULL);
	csc_htablestr_add (&ht, "Pear", NULL);
	csc_htablestr_add (&ht, "Orange", NULL);
	csc_htablestr_add (&ht, "Grape", NULL);
	csc_htablestr_add (&ht, "Lemon", NULL);
	csc_htablestr_add (&ht, "Lime", NULL);
	csc_htablestr_add (&ht, "Onion", NULL);
	ASSERT (ht.last >= 7);

	ASSERT (csc_htablestr_find (&ht, "Nonexist", NULL) < 0);
	ASSERT (csc_htablestr_find (&ht, "Apple", NULL) >= 0);
	ASSERT (csc_htablestr_find (&ht, "Banana", NULL) >= 0);
	ASSERT (csc_htablestr_find (&ht, "Pear", NULL) >= 0);
	ASSERT (csc_htablestr_find (&ht, "Orange", NULL) >= 0);
	ASSERT (csc_htablestr_find (&ht, "Grape", NULL) >= 0);
	ASSERT (csc_htablestr_find (&ht, "Lemon", NULL) >= 0);
	ASSERT (csc_htablestr_find (&ht, "Lime", NULL) >= 0);
	ASSERT (csc_htablestr_find (&ht, "Onion", NULL) >= 0);

	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Apple", NULL)), "Apple") == 0);
	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Banana", NULL)), "Banana") == 0);
	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Pear", NULL)), "Pear") == 0);
	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Orange", NULL)), "Orange") == 0);
	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Grape", NULL)), "Grape") == 0);
	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Lemon", NULL)), "Lemon") == 0);
	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Lime", NULL)), "Lime") == 0);
	ASSERT (strcmp (csc_htablestr_get_nth_name (&ht, (unsigned)csc_htablestr_find (&ht, "Onion", NULL)), "Onion") == 0);

	return EXIT_SUCCESS;
}
