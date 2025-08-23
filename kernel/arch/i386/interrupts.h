#pragma once
#include <stdint.h>
#include <stdbool.h>

// Interrupt Enable/Disable Wrappers

static inline void enable_interrupts() {
	__asm__ volatile ("sti");
}

static inline void disable_interrupts() {
	__asm__ volatile ("cli");
}

extern int timenum; // TEMP: This will be replaced when we begin working on cron/irq 0 timers.

void irq0_handler(void); // Handler for the timer interrupt.
void irq1_handler(void); // Handler for the keyboard interrupt.
void irq3_handler(void); // Handler for the breakpoint interrupt.
void irq_default_handler(void); // Handler for any other interrupts.
void pic_remap(int, int); // Function to remap the Hardware Interrupt Firmware (The PICs, obviously) using two offsets.
void idt_init(void); // Initialise the IDT to be responsive to unmasked interrupts.

// interrupts.h - The (also arch-specific) header file for interrupts.c made by the 32-bit-elf-x86, for the 32-bit-elf-x86.