#ifndef _STRING_H
#define _STRING_H 1
#include <sys/cdefs.h>
#include <stddef.h>

/* memcmp
 * PURPOSE - check if chunk a's value is smaller, equal, or larger than chunk b's value.
 * INPUT - const void* aptr (chunk a pointer), const void* bptr (chunk b pointer), size_t size (size)
 * OUPTUT - int result (-1 if chunk a is smaller, 1 if chunk a is larger, 0 if both are equal) */
int memcmp(const void*, const void*, size_t);

/* memcpy
 * PURPOSE - move a chunk of data size big from srcptr to dstptr (faster)
 * INPUT - void* dstptr (destination pointer), void* srcptr (source pointer), size_t size (size of moved chunk)
 * OUPTUT - void* dstptr (start of destination memory chunk)
 * EXCEPTION - if the dst chunk and src chunk overlap, memcpy is unpredictable and can cause major crashes.
 *             in that case, use memmove, which is slightly slower but doesn't crash. */
void* memcpy(void* __restrict, const void* __restrict, size_t);

/* memmove
 * PURPOSE - move a chunk of data size big from srcptr to dstptr (safer)
 * INPUT - void* dstptr (destination pointer), void* srcptr (source pointer), size_t size (size of moved chunk)
 * OUPTUT - void* dstptr (start of destination memory chunk) */
void* memmove(void*, const void*, size_t);

/* memset
 * PURPOSE - sets a chunk of memory to a value
 * INPUT - void* bufptr (pointer to memory), int value (value to set to), size_t size (size of set chunk)
 * OUPTUT - void* bufptr (start of memory chunk set to value) */
void* memset(void*, int, size_t);

/* strlen
 * PURPOSE - return the length of a string (in chars)
 * INPUT - const char* str (string to be measured)
 * OUPTUT - size_t length (length of str) */
size_t strlen(const char*);

#endif