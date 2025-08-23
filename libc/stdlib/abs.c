#include <stdlib.h>
#include <limits.h>

int abs(int value) {
	if (value == INT_MIN) return value;
	return value < 0 ? -value : value;
}