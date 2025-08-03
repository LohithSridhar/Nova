#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include <stdint.h>

// Interrupt Enable/Disable Wrappers

static inline void enable_interrupts() {
	__asm__ volatile ("sti");
}

static inline void disable_interrupts() {
	__asm__ volatile ("cli");
}

extern int timenum;

void irq0_handler(void);
void irq1_handler(void);
void irq_default_handler(void);
void pic_remap(int, int);
void idt_init(void);

#endif