#include <stdint.h>

// Define a GDT entry structure as per Intel manuals
struct gdt_entry {
	uint16_t limit_low;  // Lower 16 bits of limit
	uint16_t base_low; // Lower 16 bits of base
	uint8_t base_middle;  // Next 8 bits of base
	uint8_t access; // Access flags
	uint8_t granularity;  // Flags and upper 4 bits of limit
	uint8_t base_high;  // Last 8 bits of base
} __attribute__((packed));

// GDTR structure
struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void gdt_flush(struct gdt_ptr *gdt_pointer);
void gdt_set_entry(int, uint32_t, uint32_t, uint8_t, uint8_t);
void gdt_init(void);