#include <stdlib.h>
#include <limits.h>

int abs(int value) {
	if (value == INT_MIN) return value;
	return value < 0 ? -value : value;
}

// abs.c - This is a simple function that returns the absolute value of the input.