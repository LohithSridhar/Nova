#define GDT_ENTRIES 3
#include "gdt.h"

struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gdtp;

void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[num].base_low = base & 0xFFFF;
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	gdt[num].limit_low = limit & 0xFFFF;
	gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
	gdt[num].access = access;
	return;
}

void gdt_init(void) {
	gdtp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
	gdtp.base = (uint32_t)&gdt;

	// Null descriptor
	gdt_set_entry(0, 0, 0, 0, 0);

	// Code segment: base=0, limit=4GB, access=0x9A, gran=0xCF (flat, code, ring 0)
	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	// Data segment: base=0, limit=4GB, access=0x92, gran=0xCF (flat, data, ring 0)
	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	// Assembly routine to load the new GDT
	gdt_flush(&gdtp);
	return;
}