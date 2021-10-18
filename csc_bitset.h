#pragma once


#define BITSET64_ADD(x,c) ((x)[(uint64_t)(c) >> 6] |= (UINT64_C(1) << ((c) & 63)))
#define BITSET64_REMOVE(x,c) ((x)[(uint64_t)(c) >> 6] &= ~(UINT64_C(1) << ((c) & 63)))
#define BITSET64_GET(x,c) (((x)[(uint64_t)(c) >> 6] >> ((c)&63)) & 1)


