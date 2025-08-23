#include <kernel/neutrino.h>
#include <stdint.h>
#include <stdio.h>
#include <drivers/keyboard.h>
#include <drivers/tty.h>
#include "io.h"
#include "interrupts.h"

typedef struct regs {
	uint32_t gs, fs, es, ds;                          // pushed old segment registers
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pusha
	uint32_t int_no, err_code;                        // pushed by ISR
	uint32_t eip, cs, eflags, useresp, ss;            // pushed by CPU
} regs_t;

static void print_regs_to_screen(regs_t *r) {
	printf("EAX=%08X EBX=%08X ECX=%08X EDX=%08X\n", r->eax, r->ebx, r->ecx, r->edx);
	printf("ESI=%08X EDI=%08X EBP=%08X ESP=%08X\n", r->esi, r->edi, r->ebp, r->esp);
	printf("EIP=%08X CS=%08X  EFLAGS=%08X\n", r->eip, r->cs, r->eflags);
}

inline void print_registers(void) {
	regs_t r;
	__asm__ volatile (
		"mov %%gs, %0\n\t" : "=r"(r.gs)
	);
	__asm__ volatile (
		"mov %%fs, %0\n\t" : "=r"(r.fs)
	);
	__asm__ volatile (
		"mov %%es, %0\n\t" : "=r"(r.es)
	);
	__asm__ volatile (
		"mov %%ds, %0\n\t" : "=r"(r.ds)
	);
	__asm__ volatile (
		"mov %%edi, %0\n\t" : "=r"(r.edi)
	);
	__asm__ volatile (
		"mov %%esi, %0\n\t" : "=r"(r.esi)
	);
	__asm__ volatile (
		"mov %%ebp, %0\n\t" : "=r"(r.ebp)
	);
	__asm__ volatile (
		"mov %%esp, %0\n\t" : "=r"(r.esp)
	);
	__asm__ volatile (
		"mov %%ebx, %0\n\t" : "=r"(r.ebx)
	);
	__asm__ volatile (
		"mov %%edx, %0\n\t" : "=r"(r.edx)
	);
	__asm__ volatile (
		"mov %%ecx, %0\n\t" : "=r"(r.ecx)
	);
	__asm__ volatile (
		"mov %%eax, %0\n\t" : "=r"(r.eax)
	);
	__asm__ volatile (
		"pushf\n\tpop %0" : "=r"(r.eflags)
	);
	// int_no, err_code, cs, ss, eip, useresp can't be meaningfully captured here
	// unless called from an ISR — later on, expand Neutrino to be able to use them.
	r.int_no = 3;
	r.err_code = 0;
	r.eip = 0;
	r.cs = 0;
	r.useresp = 0;
	r.ss = 0;

	print_regs_to_screen(&r);
}

void print_stack() {
	uint32_t ext_base_ptr = 0;
	uint32_t ext_stack_ptr = 0;
	__asm__ volatile (
		"mov %%ebp, %0\n\t" : "=r"(ext_base_ptr)
	);
	__asm__ volatile (
		"mov %%esp, %0\n\t" : "=r"(ext_stack_ptr)
	);

	print_memory((void *) ext_stack_ptr, (size_t) (ext_base_ptr - ext_stack_ptr));
}

void neutrino_init(void) { // We only need this because we need to preserve a lot of things while resetting others.
	tty_save_screen();
	disable_interrupts();                        // External: asm cli

	idt_init();
	pic_remap(0x20, 0x28);                       // Remap PIC

	// Only unmask desired IRQs (timer, keyboard)
	outb(0x21, 0xFC);                            // Master PIC: 11111100b = enable IRQ0, IRQ1
	outb(0xA1, 0xFF);                            // Slave PIC: mask all

	enable_interrupts();                         // External: asm sti

	tty_initialize();
	keyboard_debug_init();
	return;
}

void neutrino_fini(void) {
	tty_load_screen(); // This is literally only becuase Neutrino is too high-level for that kind of driver manipulation.
	return;
}

// neutrino_i386.c - This is the file responsible for handling all architecture-specific commands in the IKD, Neutrino.
// It's functions use 32-bit x86-related commands to print important regions of memory and registers, and init/fini Neutrino.