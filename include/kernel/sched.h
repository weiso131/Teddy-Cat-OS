#pragma once
#include <type.h>
#include <arch/riscv32/page.h>
#include <kernel/util.h>

#define TIME_SLICE 1000000 //100ms

extern struct task *current;

int create_process(uintptr_t func, size_t size);

void init_schedule();

struct task *schedule();

#define create_kernel_process(func) create_process((uintptr_t)func, 0)
#define create_user_process(start, size) create_process((uintptr_t)start, (size_t) size)
