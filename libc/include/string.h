#pragma once

#include <sys/cdefs.h>
#include <stddef.h>

/* memcmp
 * PURPOSE - check if chunk a's value is smaller, equal, or larger than chunk b's value.
 * INPUT - const void *aptr (chunk a pointer), const void *bptr (chunk b pointer), size_t size (size)
 * OUTPUT - int result (-1 if chunk a is smaller, 1 if chunk a is larger, 0 if both are equal) */
int memcmp(const void*, const void*, size_t);

/* memcpy
 * PURPOSE - move a chunk of data size big from srcptr to dstptr (faster)
 * INPUT - void *dstptr (destination pointer), void *srcptr (source pointer), size_t size (size of moved chunk)
 * OUTPUT - void *dstptr (start of destination memory chunk)
 * EXCEPTION - if the dst chunk and src chunk overlap, memcpy is unpredictable and can cause major crashes.
 *             in that case, use memmove, which is slightly slower but doesn't crash. */
void *memcpy(void *__restrict, const void *__restrict, size_t);

/* memmove
 * PURPOSE - move a chunk of data size big from srcptr to dstptr (safer)
 * INPUT - void *dstptr (destination pointer), void *srcptr (source pointer), size_t size (size of moved chunk)
 * OUTPUT - void *dstptr (start of destination memory chunk) */
void *memmove(void*, const void*, size_t);

/* memset
 * PURPOSE - sets a chunk of memory to a value
 * INPUT - void *bufptr (pointer to memory), int value (value to set to), size_t size (size of set chunk)
 * OUTPUT - void *bufptr (start of memory chunk set to value) */
void *memset(void*, int, size_t);

/* memchr
 * PURPOSE - looks through a chunk of memory one by one for a value (the int)
 * INPUT - const void *ptr (pointer to memory), int value (value to search for), size_t size (size of searched chunk)
 * OUTPUT - void *strptr (pointer to first address that has the value, NULL if nothing is found) */
void *memchr(const void*, int, size_t);

/* strlen
 * PURPOSE - return the length of a string (in chars)
 * INPUT - const char *str (string to be measured)
 * OUTPUT - size_t length (length of str) */
size_t strlen(const char*);

/* strcmp
 * PURPOSE - Compares two strings (NOT memory chunks like memcmp). This includes literals, which is a bonus.
 * INPUT - const char *astr (string a), const char *bstr (string b)
 * OUTPUT - int result (-1 if string a is smaller, 1 if string b is smaller, and 0 if equal) */
int strcmp(const char*, const char*);

/* strcpy
 * PURPOSE - move a string from srcstr to dststr
 * INPUT - void *dststr (destination string), void *srcstr (source string)
 * OUTPUT - void *dststr (start of destination string)
 * EXCEPTION - if the dst string and src string overlap, strcpy is unpredictable and can cause major crashes.
 *             in that case, skill issue. */
char *strcpy(char*, const char*);

/* strcat
 * PURPOSE - concatenate two strings
 * INPUT - char *dststr (first string, also final string one this is done), const char *srcstr (second string)
 * OUTPUT - char *dststr (literally the same thing as dststr)
 * EXCEPTION - if srcstr or dststr are non-terminated, overlap, or can't fit in dststr, undefined behaviour occurs. */
char *strcat(char*, const char*);

/* strchr
 * PURPOSE - return a pointer to the first instance of a character in a string
 * INPUT - const char *str (string to search), int value (value to search for)
 * OUTPUT - char *result (pointer to the first instance, NULL/0 if not found) */
char *strchr(const char*, int);

/* strncat
 * PURPOSE - concatenate two strings together, with only length amount of the second string included
 * INPUT - char *dststr (destination and first string), const char *srcstr (second string), size_t length (amount to cat)
 * OUTPUT - char *result (again, the same as dststr)
 * EXCEPTION - if srcstr or dststr are non-terminated, overlap, or can't fit in dststr, undefined behaviour occurs. */
char *strncat(char *, const char *, size_t);

/* strcspn
 * PURPOSE - find the index of the first instance of a character in the second string in the first
 * INPUT - const char *astr (string to get index for), const char *bstr (string to check against)
 * OUTPUT - int index (index in astr of the value) */
int strcspn(const char *, const char *);

/* strncmp
 * PURPOSE - Compares two strings (NOT memory chunks like memcmp) up until length size (or the end of the string)
 * INPUT - const char *astr (string a), const char *bstr (string b), size_t length
 * OUTPUT - int result (-1 if string a is smaller, 1 if string b is smaller, and 0 if equal) */
int strncmp(const char*, const char*, size_t);

/* strncpy
 * PURPOSE - move a string length big from srcstr to dststr
 * INPUT - void *dststr (destination string), void *srcstr (source string), size_t length (length of moved string)
 * OUTPUT - void *dststr (start of destination string)
 * EXCEPTION - if the dst string and src string overlap, strncpy is unpredictable and can cause major crashes.
 *             in that case, skill issue. The same if size_t is larger than dststr. */
char *strncpy(char*, const char*, size_t);

/* strpbrk
 * PURPOSE - find a pointer to the first instance of a character in the second string in the first
 * INPUT - const char *astr (string to get index for), const char *bstr (string to check against)
 * OUTPUT - char *ptr (pointer to first instance of a bstr char in astr) */
char *strpbrk(const char *, const char *);

/* strrchr
 * PURPOSE - return a pointer to the last instance of a character in a string
 * INPUT - const char *str (string to search), int value (value to search for)
 * OUTPUT - char *result (pointer to the last instance, NULL/0 if not found) */
char *strrchr(const char*, int);

/* strspn
 * PURPOSE - find the index of the first instance of a character NOT in the second string in the first
 * INPUT - const char *astr (string to get index from), const char *bstr (string to check against)
 * OUTPUT - int index (index in astr of the first value not in bstr) */
int strspn(const char *, const char *);

/* strstr
 * PURPOSE - find the pointer to the first instance of a substring in another string
 * INPUT - const char *supstr (the string to check through), const char *substr (the string to check for)
 * OUTPUT - char *index (the pointer to the first instance of substr in supstr) */
char *strstr(const char *, const char *);

/* strtok
 * PURPOSE - tokenize a string (split it up into little pieces)
 * INPUT - char *str (string to tokenize), const char *delim (deliminations to tokenize with)
 * OUTPUT - char *str_next (I... I genuinely have no idea, I just followed a code flowchart) */
char *strtok(char*, const char*);

// string.h - This is one of the libc libraries, designed for easy handling of strings and major chunks of memory. It's one
// of the major c libraries, and also one of the ropes of the "noose" C gives you to hang yourself. Segfault here we come!