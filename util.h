#pragma once
#include "type.h"

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

inline static void *memset(void *buf, char c, size_t n) {
    uint8_t *p = (uint8_t *) buf;
    if (n < SYSTEM_ARCH_BITS / 8)
        goto small;
    
    uintptr_t mask = c | ((uintptr_t)c << 8);
    mask = mask | (mask << 16);
    if (SYSTEM_ARCH_BITS == 64)
        mask = mask | (mask << 32);
    for (;n >= SYSTEM_ARCH_BYTES;n -= SYSTEM_ARCH_BYTES, p += SYSTEM_ARCH_BYTES)
        *((uintptr_t*)p) = mask;
small:
    while (n--)
        *p++ = c;
    return buf;
}

void sbi_putchar(char ch);

long sbi_getchar(void);

void printf(const char *fmt, ...);
