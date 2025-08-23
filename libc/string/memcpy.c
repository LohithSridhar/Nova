#include <string.h>

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

// memcpy.c - One of the two memory movement functions, memcpy doesn't take care to watch out for overlaps, which makes it
// significantly faster than its paranoid sibling, memmove, but prone to stomping over values when dstptr and srcptr overlap.