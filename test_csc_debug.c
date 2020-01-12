#include "csc_debug.h"


void foo (char * str)
{
	ASSERT_PARAM_NOTNULL (str);
	puts (str);
}


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);
	foo (NULL);
	return EXIT_SUCCESS;
}
