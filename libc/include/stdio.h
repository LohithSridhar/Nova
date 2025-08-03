#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#define EOF (-1) // EOF as in End Of File

/* printf
 * PURPOSE - print string to screen with extar params formatted in
 * INPUT - const char* __restrict (string to be printed), va_args (anything to be formatted in)
 * OUPTUT - int success_code (what it sounds like)
 * EXCEPTIONS - -1 outputted (see TODO in c file) if any error 
 * TODO: currently, it prints to the screen. While this is correct, we want printf to emulate
 * vprintf, which prints to any stream necessary, instead of just writing to stdout/screen. */
int printf(const char* __restrict, ...);

/* putchar
 * PURPOSE - print a character to the screen
 * INPUT - int ic (integer version of character)
 * OUPTUT - int success_code (always ic) */
int putchar(int);

/* puts
 * PURPOSE - (for now) it's just printf with a \n at the end and no va_args
 * INPUT - const char* string (string to be printed)
 * OUPTUT - int success_code (literally just the success_code of printf) */
int puts(const char*);

int getchar(void);

char* fgets(char*, size_t);

int scanf(const char*, ...);

#endif
