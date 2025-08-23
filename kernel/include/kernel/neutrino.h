#pragma once
#include <stddef.h>

/* print_registers
 * PURPOSE - Print all of the registers respective to the CPU architecture
 * INPUT - NULL
 * OUTPUT - NULL (the registers are printed) */
void print_registers(void);

/* print_stack
 * PURPOSE - Print the memory region of the stack currently in use
 * INPUT - NULL
 * OUTPUT - NULL (stack is printed) */
void print_stack(void);

/* print_memory
 * PURPOSE - Print a memory region specified by the arguments
 * INPUT - void *address (the memory address to print), size_t length (the amount of memory to print)
 * OUTPUT - NULL (the memory is printed) */
void print_memory(void *, size_t);

/* enter_neutrino
 * PURPOSE - Enter Neutrino itself
 * INPUT - NULL
 * OUTPUT - NULL (Neutrino is entered) */
void enter_neutrino(void);

// neutrino.h - The interface for Neutrino. More accurately, the interface between arch-specific and arch-agnostic Neutrino.