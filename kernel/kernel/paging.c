#include <stdint.h>
#include <kernel/paging.h>

#define PAGE_SIZE 4096
#define ENTRIES_PER_TABLE 1024  // 4MB per page table (1024 * 4KB)

static uint32_t page_directory[1024] __attribute__((aligned(4096)));

static uint32_t page_tables_array[16][1024] __attribute__((aligned(4096))); // Support up to 16 tables

extern const uint32_t PAGE_PRESENT;
extern const uint32_t PAGE_READWRITE;
extern const uint32_t PAGE_USER;

void setup_page_tables(uint32_t num_page_tables) {
	if (num_page_tables > 16) {
		// limit number of page tables to avoid overflow
		num_page_tables = 16;
	}

	for (uint32_t table_index = 0; table_index < num_page_tables; table_index++) {
		for (uint32_t i = 0; i < ENTRIES_PER_TABLE; i++) {
			uint32_t phys_addr = (table_index * ENTRIES_PER_TABLE + i) * PAGE_SIZE;
			page_tables_array[table_index][i] = phys_addr | PAGE_PRESENT | PAGE_READWRITE;
		}

		// Point page directory entries to each page table base address
		page_directory[table_index] = ((uint32_t)(&page_tables_array[table_index])) | PAGE_PRESENT | PAGE_READWRITE;
	}

	// Zero out remaining page directory entries
	for (uint32_t i = num_page_tables; i < 1024; i++) {
		page_directory[i] = 0;
	}
}

uint32_t *get_page_directory(void) {
	return page_directory;
}

void paging_initialize(void) {
	setup_page_tables(10);
	enable_paging();
}