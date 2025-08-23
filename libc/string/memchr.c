#include <stddef.h>
#include <string.h>

void *memchr(const void *ptr, int value, size_t size) {
	unsigned char *strptr = (unsigned char *) ptr;
	for (size_t i = 0; i < size; i++) {
		if (strptr[i] == value) return (void *) (ptr + i);
	}

	return NULL;
}

// memchr.c - This function takes in a memory location with a size, and scrolls through it until it finds a value, and returns
// a pointer to the address of the first instance of that value.