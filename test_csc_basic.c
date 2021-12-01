#include "csc_basic.h"
#include "csc_assert.h"
#include "csc_crossos.h"
#include "csc_strf.h"






int main (int argc, char const * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);


	return EXIT_SUCCESS;
}
