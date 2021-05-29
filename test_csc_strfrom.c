#include "csc_strfrom.h"

#define STR_LENGTH 20


void hr()
{
	printf ("==============================\n");
}

int main (int argc, char * argv [])
{
	ASSERT (argc);
	ASSERT (argv);

	{
		char str[STR_LENGTH] = {0};
		strfrom_imax (str, STR_LENGTH, 123456789, 10, '$');
		fwrite (str, 1, sizeof(str), stdout);
		printf ("\n");
	}

	hr();

	{
		char str[STR_LENGTH] = {0};
		strfrom_imax2 (str, STR_LENGTH, 123456789, 10);
		fwrite (str, 1, sizeof(str), stdout);
		printf ("\n");
	}

	hr();

	{
		char str[STR_LENGTH] = {0};
		strfrom_imax2 (str, STR_LENGTH, 123456789, -10);
		fwrite (str, 1, sizeof(str), stdout);
		printf ("\n");
	}

	return EXIT_SUCCESS;
}
