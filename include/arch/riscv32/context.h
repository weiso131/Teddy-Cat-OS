#pragma once
#include <type.h>
#include "page.h"

inline static void switch_context(struct task *current, struct task *next, uint32_t sp)
{
    current->sp = sp;
    __asm__ __volatile__(
        "csrr t0, sepc\n"
        "mv %0, t0\n"
        "csrw sepc, %1\n"
        "mv sp, %2\n"
        "sfence.vma\n"
        "csrw satp, %3\n"
        "sfence.vma\n"
        : "=&r" (current->sepc)
        : "r" (next->sepc), "r" (next->sp), "r" (SATP_SV32 | ((uint32_t) next->page_table / PAGE_SIZE))
        : "t0", "memory"
    );    
}

inline static void start_task(struct task *current)
{
    __asm__ __volatile__(
        "mv sp, %0\n"
        "mv ra, %1\n"
        "ret\n"
        :
        : "r" (current->kernel_stack + (1 << 12)), "r" (current->sepc)
        : 
    );
}
