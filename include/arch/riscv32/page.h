#pragma once
#include <type.h>

#define SATP_SV32 (1u << 31)

#define PAGE_V (1 << 0)
#define PAGE_R (1 << 1)
#define PAGE_W (1 << 2)
#define PAGE_X (1 << 3)
#define PAGE_U (1 << 4)
#define PAGE_G (1 << 5)
#define PAGE_A (1 << 6)
#define PAGE_D (1 << 7)

#define PAGE_SIZE 4096
#define PAGE_SIZE_BITS 12

void init_heap(void);

/* allocate one page(4 KB) memory */
char *alloc_page();

void free_page(char *p);

void set_page_table(uint32_t page_table);

void map_vpage(uint32_t *page_table1, uint32_t v_addr, uint32_t p_addr, uint8_t flags);
