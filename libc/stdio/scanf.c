#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Helper: parse hex integer (lower or upper case)
static int parse_hex(const char *s, int *out) {
	int val = 0, read = 0;
	while (*s == ' ' || *s == '\t') s++;
	if (*s == '0' && (s[1] == 'x' || s[1] == 'X')) s += 2;
	while (1) {
		char c = *s;
		if (c >= '0' && c <= '9') val = (val << 4) + (c - '0');
		else if (c >= 'a' && c <= 'f') val = (val << 4) + (c - 'a' + 10);
		else if (c >= 'A' && c <= 'F') val = (val << 4) + (c - 'A' + 10);
		else break;
		s++; read = 1;
	}
	if (read) *out = val;
	return read;
}

int scanf(const char *format, ...) {
	char buf[256];
	if (!fgets(buf, sizeof(buf))) return 0;
	va_list ap;
	va_start(ap, format);
	const char *f = format;
	const char *s = buf;
	int assigned = 0;

	while (*f) {
		while (*f && *f != '%') { // skip ordinary chars
			if (*f == *s) {f++, s++;}
			else if (*f == ' ' || *f == '\t') {while (*s == ' ') s++; f++;} // we don't allow tab
			else break;
		}
		if (*f == '%' && f[1]) {
			f++;
			if (*f == 'd') {
				int *ptr = va_arg(ap, int *);
				if (!(((*s == '-' || *s == '+') && isdigit(*(s+1))) || isdigit(*s))) break;
				*ptr = atoi(s);
				assigned++;
				while ((*s >= '0' && *s <= '9') || *s == '-' || *s == '+') s++;
			} else if (*f == 'x' || *f == 'X') {
				int *ptr = va_arg(ap, int *);
				int val;
				if (!parse_hex(s, &val)) break;
				*ptr = val;
				assigned++;
				// skip hex digits
				while ((*s >= '0' && *s <= '9') ||
					   (*s >= 'a' && *s <= 'f') ||
					   (*s >= 'A' && *s <= 'F') ||
					   *s == 'x' || *s == 'X') s++;
			} else if (*f == 's') {
				char *ptr = va_arg(ap, char *);
				while (*s == ' ' || *s == '\t') s++;
				int i = 0;
				while (*s && *s != ' ' && *s != '\t' && *s != '\n') { ptr[i++] = *s++; }
				ptr[i] = 0;
				if (i) assigned++;
				else break;
			} else if (*f == 'c') {
				char *ptr = va_arg(ap, char *);
				if (*s) { *ptr = *s++; assigned++; }
				else break;
			}
			f++;
		} else {
			break;
		}
	}
	va_end(ap);
	return assigned;
}

// scanf.c - This function (or rather, set of functions) takes in a format string and some variables' pointers to output to,
// and uses pointer referencing to change variables in order to take in some input and parse it into program data.