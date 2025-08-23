#include <string.h>

int memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

// memcmp.c - This function takes in two memory locations with the same size, and compares them until there's a difference,
// at which point it reports the difference. Note that it works with void *, a memory pointer, and not char *, a string.