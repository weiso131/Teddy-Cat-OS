#pragma once

void init_heap(void);

/* allocate one page(4 KB) memory */
char *alloc_page();

void free_page(char *p);
