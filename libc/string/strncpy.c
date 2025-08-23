#include <stdbool.h>
#include <string.h>

char *strncpy(char *dststr, const char *srcstr, size_t length) {
	bool pad = false;
	for (int i = 0; i < length; i++) {
		if (pad) {dststr[i] = '\0'; continue;}
		if (!srcstr[i]) {pad = true; dststr[i] = '\0'; continue;}
		dststr[i] = srcstr[i];
	}

	return dststr;
}

// strncpy.c - strncpy is just strcpy, but with the extra that you can copy different sizes, and can NULL-pad everything else.