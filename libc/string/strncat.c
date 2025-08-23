#include <string.h>

char *strncat(char *dststr, const char *srcstr, size_t length) {
	size_t cat_length = length < strlen(srcstr) ? length : strlen(srcstr);
	size_t astr_length = strlen(dststr);

	for (int i = 0; i <= cat_length; i++) {
		dststr[astr_length + i] = srcstr[i];
	}

	dststr[astr_length + cat_length] = '\0';

	return dststr;
}

// strncat.c - strncat is just strcat, but with the ability to only cat n characters from srcstr.