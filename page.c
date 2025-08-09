#include "page.h"
#include "util.h"

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
    
    if (__heap_map_start + i == __heap_map_end)
        return 0;
    
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
