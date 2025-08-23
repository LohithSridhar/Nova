#include <string.h>

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

// memset.c - memset is a very simple function: It takes in a memory location, and sets all values for a set chunk SIZE big to
// the value VALUE. As with all other mem-type functions, it can stomp over memory if not used carefully.