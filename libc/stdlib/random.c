#include <stdlib.h>

static int next = 1;

void srand(int seed) {
	next = seed;
}

int rand(void) {
	next = next * 1103515245 + 12345;
	return (unsigned int) (next / 65536) % 37268;
}