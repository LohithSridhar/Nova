#include "interrupts.h"
#include "io.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <drivers/keyboard.h>
#include <kernel/kernel_init.h>
#include <kernel/neutrino.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define IDT_ENTRIES 256
#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

int timenum = 0; // temp until timers/semaphores are introd to the os

void pic_remap(int offset1, int offset2) {
	uint8_t a1 = inb(PIC1_DATA);
	uint8_t a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	outb(PIC1_DATA, offset1);
	outb(PIC2_DATA, offset2);

	outb(PIC1_DATA, 4);  // Tell Master PIC about slave at IRQ2
	outb(PIC2_DATA, 2);  // Tell Slave PIC its cascade identity

	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	outb(PIC1_DATA, a1); // restore saved masks
	outb(PIC2_DATA, a2);
}

// IDT structures
struct idt_entry {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t  zero;
	uint8_t  flags;
	uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

__attribute__((section(".idt"), aligned(16)))
struct idt_entry idt[IDT_ENTRIES];

struct idt_ptr idt_ptr;

extern void irq0_handler(void);
extern void irq1_handler(void);
extern void irq3_handler(void);
extern void irq_default_handler(void);
extern void keyboard_interrupt(void);

static inline void idt_load(void* idt_ptr) {
    __asm__ volatile ("lidt (%0)" : : "r"(idt_ptr) : "memory");
}

void irq_handler(int irq_num) {
	switch (irq_num) {
		case 0:
			if (timenum < 100) {
				timenum++;
			} else {
				timenum = 0;
			}

			if (in_a_breakpoint() && !currently_in_a_breakpoint()) breakpoint();
			break;
		case 1:
			keyboard_interrupt();
			break;
		case 3:
			enter_neutrino();
			break;
		default:
			printf("UNKNOWN INTERRUPT (NUMBER 0x%X)", irq_num);
	}
}

// IDT setup
static void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt[num].offset_low  = base & 0xFFFF;
	idt[num].selector    = sel;
	idt[num].zero        = 0;
	idt[num].flags       = flags;
	idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void idt_init(void) {
	idt_ptr.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
	idt_ptr.base  = (uint32_t)&idt;

	// Set all entries to default handler
	for (int i = 0; i < IDT_ENTRIES; i++)
		idt_set_gate(i, (uint32_t)irq_default_handler, 0x08, 0x8E);

	// Set IRQ0 (timer) and IRQ1 (keyboard) to their handlers
	idt_set_gate(32, (uint32_t)irq0_handler, 0x08, 0x8E);
	idt_set_gate(33, (uint32_t)irq1_handler, 0x08, 0x8E);
	idt_set_gate(3, (uint32_t)irq3_handler, 0x08, 0x8E);

	idt_load(&idt_ptr); // Assembly: lidt [idt_ptr]
}

// interrupts.c - The file responsible for handling interrupts on the x86 system (32-bit). It utilises the Master and Slave
// PIC (Currently only the Master) to handle the timer and keyboard, and a special software interrupt handler for the IKD.