#include "csc_strf.h"
#include "csc_crossos.h"

int main (int argc, char * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	test_csc_strf();

	return EXIT_SUCCESS;
}
