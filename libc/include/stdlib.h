#pragma once

#ifndef NULL
#define NULL ((void*)0)
#endif

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

extern uintptr_t *__stack_chk_guard;

__attribute__((__noreturn__))
void __stack_chk_fail(void);

void __ssp_init(uint32_t);

/* malloc
 * PURPOSE - Possibly THE most used funciton in C. Allocates memory to the heap.
 * INPUT - size_t size (size of region to mallocate ;D)
 * OUTPUT - void *heap_block (pointer to block of heap) */
void *malloc(size_t);

/* free
 * PURPOSE - Possibly the 3rd most used function, after malloc and printf. Frees memory from the heap.
 * INPUT - void *heap_block (pointer to block to be freed)
 * OUTPUT - int success (success_code for the freeing of the block) */
int free(void *);

/* calloc
 * PURPOSE - Continuously allocate heap memory for multiple variables
 * INPUT - size_t nmemb (amount of memory variables to allocate), size_t size (size of variables)
 * OUTPUT - void *heap_block (allocated memory) */
void *calloc(size_t, size_t);

/* realloc
 * PURPOSE - Reallocate (resize) a memory block
 * INPUT - void *prev_block (previously allocated block), size_t new_size
 * OUTPUT - void *new_block (new allocated block) */
void *realloc(void *, size_t);

#if defined(__is_libk)
/* print_heap_header_data
 * PURPOSE - Debug Print - prints the heap's header data.
 * INPUT - NULL
 * OUTPUT - NULL (the relevant data is printed) */
void print_heap_header_data(void);

/* print_heap_memory_data
 * PURPOSE - Debug Print - prints the heaps' memory regions using Neutrino's memory printer.
 * INPUT - NULL
 * OUTPUT - NULL */
void print_heap_memory_data(void);
#endif

/* abs
 * PURPOSE - Finding the absolute value of a number
 * INPUT - int value
 * OUTPUT - int abs_value (|value| in math terms) */
int abs(int);

/* atoi
 * PURPOSE - Converting a string into an integer
 * INPUT - const char *str
 * OUTPUT - int value
 * EXCEPTION - If str isn't a valid integer (eg. "hello"), atoi returns 0. 
 * NOTES - atoi accepts preceding spaces and tabs, and a -/+ before the number. */
int atoi(const char *);

/* srand
 * PURPOSE - Setting the value of the random seed
 * INPUT - int seed
 * OUTPUT - NULL (seed is set) */
void srand(int);

/* rand
 * PURPOSE - Getting a random value
 * INPUT - NULL
 * OUTPUT - int random_number */
int rand(void);

/* abort (panic in case of libk usage)
 * PURPOSE - Kernel Panic
 * INPUT - char *reason, char *process_name
 * OUTPUT - NULL */
__attribute__((__noreturn__))
void abort(const char*, const char*);

// stdlib.h - One of the libc libraries. This is concerned with core system functions involving killing apps and working with
// core heap allocation processes. It's one of the ways that you can save yourself from your own mistak- I mean segfaults.