#include "csc_str.h"



int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	{
		char const str[] = "Hello()Banana()";
		char const * p;
		p = str;
		str_skip_until (&p, "()");
		printf ("%s\n", p);
		str_skip_after (&p, ")(");
		printf ("%s\n", p);
	}

	return EXIT_SUCCESS;
}
