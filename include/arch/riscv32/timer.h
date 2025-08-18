#pragma once
#include <type.h>
#include <arch/riscv32/sbi.h>

static inline uint64_t get_time(void)
{
    uint32_t lo, hi, tmp;
    __asm__ __volatile__(
        "1:\n"
        "   rdtimeh %0\n"
        "   rdtime %1\n"
        "   rdtimeh %2\n"
        "   bne %0, %2, 1b\n"
        : "=r"(hi), "=r"(lo), "=r"(tmp)
    );
        
    return ((uint64_t)hi << 32) | lo;
}

/**
 * timer_wait() - wait for a while and trigger timer interrupt
 * @wait_time: the time waiting, in QEMU, the time is 10MHz
 * 
 * Return: struct sbiret
 */
inline static struct sbiret timer_wait(uint64_t wait_time)
{
    uint64_t now_time = get_time();
    return sbi_set_timer(now_time + wait_time);
}

inline static void timer_disable()
{
    __asm__ __volatile__(
        "csrr t0, sie\n"
        "li t1, 0xFFDF\n"
        "and t0, t0, t1\n"
        "csrw sie, t0\n"
    );
}

inline static void timer_enable()
{
    __asm__ __volatile__(
        "csrr t0, sie\n"
        "li t1, 0x20\n"
        "or t0, t0, t1\n"
        "csrw sie, t0\n"
    );
}
