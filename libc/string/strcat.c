#include <string.h>

char *strcat(char *dststr, const char *srcstr) {
	size_t dst_current_len = strlen(dststr);
	for (int i = 0; srcstr[i]; i++) {
		dststr[i+dst_current_len] = srcstr[i];
	}

	return dststr;
}

// strcat.c - strcat is a simple function that conCATenates two strings together and stores the result in the first one.