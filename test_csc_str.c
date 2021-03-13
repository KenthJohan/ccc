#include "csc_basic.h"
#include "csc_tcol.h"
#include "csc_crossos.h"

#include <stdio.h>


int main (int argc, char * argv [])
{
	UNUSED (argc);
	UNUSED (argv);
	csc_crossos_enable_ansi_color();

	for (int i = 0; i < 0xFF; ++i)
	{
		if ((i % 0xF) == 0) {printf ("\n");}
		printf (TCOLF("%i") "%02x" TCOL_RST, i, i);
	}
	printf ("\n");
	for (int i = 0; i < 0xFF; ++i)
	{
		if ((i % 0xF) == 0) {printf ("\n");}
		printf (TCOLB("%i") "%02x" TCOL_RST, i, i);
	}
	printf ("\n");
	return 0;
}
