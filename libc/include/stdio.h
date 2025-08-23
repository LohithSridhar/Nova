#pragma once

#include <sys/cdefs.h>
#include <stddef.h>

#define EOF (-1) // EOF as in End Of File

/* printf
 * PURPOSE - print string to screen with extar params formatted in
 * INPUT - const char* __restrict (string to be printed), va_args (anything to be formatted in)
 * OUTPUT - int success_code
 * EXCEPTIONS - -1 outputted (see TODO in .c file) if any error 
 * NOTE: printf (And all functions that use it) support ANSI escape characters now!
 * TODO: currently, it prints to the screen. While this is correct, we want printf to emulate
 * vprintf, which prints to any stream necessary, instead of just writing to stdout/screen. */
int printf(const char* __restrict, ...);

/* putchar
 * PURPOSE - print a character to the screen
 * INPUT - int ic (integer version of character)
 * OUTPUT - int success_code (always ic) */
int putchar(int);

/* puts
 * PURPOSE - (for now) it's just printf with a \n at the end and no va_args
 * INPUT - const char* string (string to print)
 * OUTPUT - int success_code */
int puts(const char*);

/* getchar
 * PURPOSE - Recieving keyboard input from the user.
 * INPUT - NULL
 * OUTPUT - int character */
int getchar(void);

/* fgets
 * PURPOSE - Getting a set of (raw) characters from the keyboard
 * INPUT - char *buffer (buffer to write to), size_t size (size of the buffer)
 * OUTPUT - char *result (if the read was successful, it's the buffer, else it's empty) */
char* fgets(char*, size_t);

/* scamf
 * PURPOSE - Getting a formatted input from the string
 * INPUT - const char *format (The format string for the read order), va_args (anything to be formatted up)
 * OUTPUT - int success_code (what it sounds like, yet again)
 * EXCEPTIONS - -1 outputted (see TODO in .c file) if any error */
int scanf(const char*, ...);

// stdio.h - This is possibly the most referenced file in all of C, as it outputs and inputs between the program and user.
// It's primarily known for the universal debug, printf & scanf, and for its collection of %(whatever) format strings.