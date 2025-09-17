#include <stdint.h>
#include <kernel/paging.h>

const uint32_t PAGE_PRESENT = 0x1;
const uint32_t PAGE_READWRITE = 0x2;
const uint32_t PAGE_USERMODE = 0x4;

static inline void load_cr3(uint32_t *page_directory_address) {
	asm volatile("mov %0, %%cr3" :: "r"(page_directory_address) : "memory");
}

static inline uint32_t read_cr0(void) {
	uint32_t cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	return cr0;
}

static inline void write_cr0(uint32_t cr0) {
	asm volatile("mov %0, %%cr0" :: "r"(cr0) : "memory");
}

void enable_paging(void) {
	load_cr3(get_page_directory());

	uint32_t cr0 = read_cr0();
	cr0 |= 0x80000000;
	write_cr0(cr0);
	
	return;
}