#include "csc_crossos.h"
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
	csc_crossos_enable_ansi_color();
	foo (NULL);
	return EXIT_SUCCESS;
}
