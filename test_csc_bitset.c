#include "csc_assert.h"
#include "csc_crossos.h"
#include "csc_basic.h"
#include "csc_bitset.h"
#include "csc_strf.h"


int main (int argc, char const * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	uint64_t f[2] = {0,0};
	BITSET64_ADD(f, '0');
	BITSET64_ADD(f, '1');
	BITSET64_ADD(f, 'a');
	ASSERT (f[0] == 0b0000000000000011000000000000000000000000000000000000000000000000);
	ASSERT (f[1] == 0b0000000000000000000000000000001000000000000000000000000000000000);
	BITSET64_REMOVE(f, '0');
	ASSERT (f[0] == 0b0000000000000010000000000000000000000000000000000000000000000000);
	ASSERT (f[1] == 0b0000000000000000000000000000001000000000000000000000000000000000);
	ASSERT(BITSET64_GET(f, '0') == 0);
	ASSERT(BITSET64_GET(f, '1') == 1);
	//strf_printf ("%w064u64_2\n", f[1]);


	f[0] = 0;
	f[1] = 0;
	BITSET64_ADD(f, '0');
	BITSET64_ADD(f, '9');
	BITSET64_ADD(f, 'a');
	BITSET64_ADD(f, 'A');
	BITSET64_ADD(f, 'z');
	BITSET64_ADD(f, 'Z');
	ASSERT(BITSET64_GET(f, '0') == 1);
	ASSERT(BITSET64_GET(f, '9') == 1);
	ASSERT(BITSET64_GET(f, 'a') == 1);
	ASSERT(BITSET64_GET(f, 'A') == 1);
	ASSERT(BITSET64_GET(f, 'z') == 1);
	ASSERT(BITSET64_GET(f, 'Z') == 1);
	BITSET64_REMOVE(f, '0');
	BITSET64_REMOVE(f, '9');
	BITSET64_REMOVE(f, 'a');
	BITSET64_REMOVE(f, 'A');
	BITSET64_REMOVE(f, 'z');
	BITSET64_REMOVE(f, 'Z');
	ASSERT(BITSET64_GET(f, '0') == 0);
	ASSERT(BITSET64_GET(f, '9') == 0);
	ASSERT(BITSET64_GET(f, 'a') == 0);
	ASSERT(BITSET64_GET(f, 'A') == 0);
	ASSERT(BITSET64_GET(f, 'z') == 0);
	ASSERT(BITSET64_GET(f, 'Z') == 0);
	ASSERT (f[0] == 0);
	ASSERT (f[1] == 0);



	return EXIT_SUCCESS;
}
