#pragma once
#include <stdint.h>

void setup_first_page_table(void);
void setup_page_directory(void);
uint32_t *get_page_directory(void);
void enable_paging(void);
void paging_initialize(void);

extern const uint32_t PAGE_PRESENT;
extern const uint32_t PAGE_READWRITE;
extern const uint32_t PAGE_USER;