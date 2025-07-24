#ifndef _STDLIB_H
#define _STDLIB_H 1
#include <sys/cdefs.h>

/* abort (panic in case of libk useage)
 * PURPOSE - Kernel Panic
 * INPUT - char* reason (temporary, displays reason of panic)
 * OUPTUT - NULL */
__attribute__((__noreturn__))
void abort(char*);

#endif