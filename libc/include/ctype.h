#pragma once

// Accepts int c, expects EOF or unsigned char range [0..255]

static inline int isalpha(int c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

static inline int isdigit(int c) {
	return (c >= '0' && c <= '9');
}

static inline int isalnum(int c) {
	return (isalpha(c) || isdigit(c));
}

static inline int islower(int c) {
	return (c >= 'a' && c <= 'z');
}

static inline int isupper(int c) {
	return (c >= 'A' && c <= 'Z');
}

static inline int isxdigit(int c) {
	return (isdigit(c) ||
		   (c >= 'a' && c <= 'f') ||
		   (c >= 'A' && c <= 'F'));
}

static inline int isspace(int c) {
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\r' || c == '\v' || c == '\f');
}

static inline int ispunct(int c) {
	return ( (c >= 33 && c <= 47) ||
			 (c >= 58 && c <= 64) ||
			 (c >= 91 && c <= 96) ||
			 (c >= 123 && c <= 126));
}

static inline int isprint(int c) {
	return (c >= 32 && c < 127);
}

static inline int isgraph(int c) {
	return (isprint(c) && c != ' ');
}

static inline int iscntrl(int c) {
	return (c >= 0 && c < 32) || (c == 127);
}

static inline int isblank(int c) {
	return (c == ' ' || c == '\t');
}

// Convert uppercase to lowercase, ASCII only
static inline int tolower(int c) {
	if (isupper(c)) return c + ('a' - 'A');
	return c;
}

// Convert lowercase to uppercase, ASCII only
static inline int toupper(int c) {
	if (islower(c)) return c - ('a' - 'A');
	return c;
}

// ctype.h - This is a header file containing a lot of little utilities that we can use to work with comparisons a lot better!