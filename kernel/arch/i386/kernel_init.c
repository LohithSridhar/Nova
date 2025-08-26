#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <drivers/tty.h>
#include <drivers/keyboard.h>
#include "io.h"
#include "interrupts.h"
#include <kernel/kernel_init.h>

extern void heap_initialize(void);

#define GDT_ENTRIES 4
#define KERNEL_STACK_TOP 0x200000

static bool in_breakpoint = false;
static bool currently_in_breakpoint = false;

static inline void set_kernel_stack(void) {
	__asm__ volatile ("movl %0, %%esp" : : "r"(KERNEL_STACK_TOP) : );
}

// GDT structures
struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

// TSS (for privilege stack switch, required for safe interrupt/trap handling)
struct tss_entry {
	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t unused1[23];
} __attribute__((packed, aligned(16)));

// Place GDT and TSS in their own sections (update your linker!)
// Example linker script lines:
// .gdt  : { *(.gdt)  }  /* address set with . = ALIGN(0x1000); before */
// .tss  : { *(.tss)  }
__attribute__((section(".gdt"), aligned(16)))
struct gdt_entry gdt[GDT_ENTRIES];

__attribute__((section(".tss"), aligned(16)))
struct tss_entry tss;

struct gdt_ptr gdtp;

// External assembly hooks
extern void gdt_flush(void *gdt_ptr);
extern void tss_flush(uint16_t tss_selector); // if not inline, otherwise see below

// GDT setup helpers
static void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[num].base_low     = base & 0xFFFF;
	gdt[num].base_middle  = (base >> 16) & 0xFF;
	gdt[num].base_high    = (base >> 24) & 0xFF;
	gdt[num].limit_low    = limit & 0xFFFF;
	gdt[num].granularity  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
	gdt[num].access       = access;
}

// TSS GDT entry (NOTE: granularity must NOT include gran flags, only high nibble of limit)
static void gdt_set_tss(int num, uint32_t base, uint32_t limit) {
	gdt[num].base_low    = base & 0xFFFF;
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high   = (base >> 24) & 0xFF;
	gdt[num].limit_low   = limit & 0xFFFF;
	gdt[num].granularity = (limit >> 16) & 0x0F;  // no flags (use 0x00 for TSS!)
	gdt[num].access      = 0x89;                  // Present, ring 0, 32-bit available TSS
}

static void gdt_init(void) {
	gdtp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
	gdtp.base  = (uint32_t)&gdt;

	gdt_set_entry(0, 0, 0, 0, 0);                           // Null descriptor
	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);            // Code segment
	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);            // Data segment

	// Setup TSS struct and its descriptor
	memset(&tss, 0, sizeof(tss));
	tss.ss0  = 0x10;              // Data segment selector (GDT entry 2)
	tss.esp0 = 0x200000;          // Example kernel stack top (ensure this is mapped/valid)
	gdt_set_tss(3, (uint32_t)&tss, sizeof(tss)-1);

	gdt_flush(&gdtp);             // Assembly: lgdt, reload segments (and far jump for cs)
	// Inline or external: tss_flush(0x18); // 0x18 = GDT entry 3
	__asm__ volatile ("ltr %0" : : "r"((uint16_t)0x18)); // Load TSS selector (entry 3)
}

void set_in_breakpoint_true() {
	in_breakpoint = true;
}

bool in_a_breakpoint() {
	return in_breakpoint;
}

bool currently_in_a_breakpoint() { // Why??????????
	return currently_in_breakpoint;
}

inline void breakpoint(void) {
	in_breakpoint = true;
	currently_in_breakpoint = true;
	__asm__ volatile ("int $0x3");
	in_breakpoint = false;
	currently_in_breakpoint = false;
}

extern void _init(void);

void random_init(void) { // lmao this is so simple
	srand(timenum);
}

void kernel_init(void) {
	_init(); // Call global constructors

	disable_interrupts();                        // External: asm cli

	set_kernel_stack();                          // Set esp to point to proper kernel stack

	gdt_init();
	idt_init();
	pic_remap(0x20, 0x28);                       // Remap PIC

	// Only unmask desired IRQs (timer, keyboard)
	outb(0x21, 0xFC);                            // Master PIC: 11111100b = enable IRQ0, IRQ1
	outb(0xA1, 0xFF);                            // Slave PIC: mask all

	enable_interrupts();                         // External: asm sti

	// Kernel main loop or hand off to further init...
	heap_initialize();
	tty_initialize();
	keyboard_initialize();
	return;
}

void wait(void) {
	while (timenum != 0) {};
	return;
}

extern void _fini(void);

void kernel_exit(void) {
	_fini(); // Call Global Destructors
	// This is where shutdown or sleep processes will occur. For now, this is empty.
	return;
}

__attribute__((__noreturn__))
void halt(void) {
	disable_interrupts();
	while (1) {};
	__builtin_unreachable();
}

// kernel_init.c - While it's called kernel_init, it (currently) covers everything from random numbers to halting the kernel.