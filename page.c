#include "page.h"
#include "util.h"
#include "csr.h"

extern uintptr_t __heap_map_start[], __heap_map_end[];
extern char __heap_start[], __heap_end[];


void init_heap(void)
{
    memset(__heap_start, 0, __heap_end - __heap_start);
}

/* allocate one page(4 KB) memory */
char *alloc_page()
{
    uintptr_t i = 0;
    for (;__heap_map_start[i] == 0xFFFFFFFF && \
            __heap_map_start + i != __heap_map_end;i++);
    
    if (__heap_map_start + i == __heap_map_end) {
        PANIC("RAM is full\n");
    }
    
    uintptr_t j = 0;
    for (;(__heap_map_start[i] >> j) & 1;j++);

     __heap_map_start[i] |= 1 << j;

    
    return __heap_start + (((i << 5) + j) << 12);

}


void free_page(char *p)
{
    uintptr_t offset = ((uintptr_t)p - (uintptr_t)__heap_start) >> 12;

    uintptr_t i = offset >> 5, j = offset & 0x1F;;
    __heap_map_start[i] &= ~(1 << j);

}

void set_page_table(uint32_t page_table)
{   
    if (is_aligned(page_table, PAGE_SIZE_BITS)) 
        PANIC("page_table need to align to page size.");
    WRITE_CSR(satp, page_table >> PAGE_SIZE_BITS);
}   

void map_vpage(uint32_t *page_table1, uint32_t v_addr, uint32_t p_addr, uint8_t flags)
{
    if (!is_aligned(v_addr, PAGE_SIZE_BITS)) 
        PANIC("v_addr need to align to page size.");
    if (!is_aligned(p_addr, PAGE_SIZE_BITS)) 
        PANIC("p_addr need to align to page size.");

    uint32_t vpn1 = v_addr >> 22;
    uint32_t vpn0 = (v_addr >> 12) & 0x3FF;

    if (!(page_table1[vpn1] & PAGE_V)) {
        uint32_t page_table = (uint32_t)alloc_page();
        page_table1[vpn1] = ((page_table / PAGE_SIZE) << 10) | PAGE_V;
    }

    uint32_t *page_table0 = (uint32_t *)((page_table1[vpn1] >> 10) * PAGE_SIZE);;
    page_table0[vpn0] = ((p_addr / PAGE_SIZE) << 10) | PAGE_V | flags;
}
