#include <stdlib.h>
#include <limits.h>

int atoi(const char *s) {
	int val = 0, sign = 1, read = 0;
	while (*s == ' ' || *s == '\t') s++;
	if (*s == '-') { sign = -1; s++; }
	else if (*s == '+') s++;
	while (*s >= '0' && *s <= '9') {
		if (val >= (INT_MAX / 10)) return sign == -1 ? INT_MIN : INT_MAX;
		val = val * 10 + (*s - '0');
		s++;
		read = 1;
	}
	if (read) return val * sign;
	return 0; // Invalid output
}

// atoi.c - This function is a utility designed to convert strings into numbers.