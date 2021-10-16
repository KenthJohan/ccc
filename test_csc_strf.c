#include "csc_strf.h"


int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	{
		char buf[10] = {'\0'};
		strf_fmt (buf, 10, "%u32_10", 3);
		puts (buf);
	}

	return EXIT_SUCCESS;
}
