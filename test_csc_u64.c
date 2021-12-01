#include "csc_basic.h"
#include "csc_assert.h"
#include "csc_crossos.h"
#include "csc_strf.h"
#include "csc_u64.h"



static void test_VU64_BITSET()
{
	uint64_t f[2] = {0,0};
	VU64_BITSET_ADD(f, '0');
	VU64_BITSET_ADD(f, '1');
	VU64_BITSET_ADD(f, 'a');
	ASSERT (f[0] == 0b0000000000000011000000000000000000000000000000000000000000000000);
	ASSERT (f[1] == 0b0000000000000000000000000000001000000000000000000000000000000000);
	VU64_BITSET_DEL(f, '0');
	ASSERT (f[0] == 0b0000000000000010000000000000000000000000000000000000000000000000);
	ASSERT (f[1] == 0b0000000000000000000000000000001000000000000000000000000000000000);
	ASSERT(VU64_BITSET_GET(f, '0') == 0);
	ASSERT(VU64_BITSET_GET(f, '1') == 1);
	f[0] = 0;
	f[1] = 0;
	VU64_BITSET_ADD(f, '0');
	VU64_BITSET_ADD(f, '9');
	VU64_BITSET_ADD(f, 'a');
	VU64_BITSET_ADD(f, 'A');
	VU64_BITSET_ADD(f, 'z');
	VU64_BITSET_ADD(f, 'Z');
	ASSERT(VU64_BITSET_GET(f, '0') == 1);
	ASSERT(VU64_BITSET_GET(f, '9') == 1);
	ASSERT(VU64_BITSET_GET(f, 'a') == 1);
	ASSERT(VU64_BITSET_GET(f, 'A') == 1);
	ASSERT(VU64_BITSET_GET(f, 'z') == 1);
	ASSERT(VU64_BITSET_GET(f, 'Z') == 1);
	VU64_BITSET_DEL(f, '0');
	VU64_BITSET_DEL(f, '9');
	VU64_BITSET_DEL(f, 'a');
	VU64_BITSET_DEL(f, 'A');
	VU64_BITSET_DEL(f, 'z');
	VU64_BITSET_DEL(f, 'Z');
	ASSERT(VU64_BITSET_GET(f, '0') == 0);
	ASSERT(VU64_BITSET_GET(f, '9') == 0);
	ASSERT(VU64_BITSET_GET(f, 'a') == 0);
	ASSERT(VU64_BITSET_GET(f, 'A') == 0);
	ASSERT(VU64_BITSET_GET(f, 'z') == 0);
	ASSERT(VU64_BITSET_GET(f, 'Z') == 0);
	ASSERT (f[0] == 0);
	ASSERT (f[1] == 0);
	//strf_printf ("%w064u64_2\n", f[1]);
}

int main (int argc, char const * argv [])
{
	csc_crossos_enable_ansi_color();
	ASSERT (argc);
	ASSERT (argv);

	test_VU64_BITSET();


	return EXIT_SUCCESS;
}
