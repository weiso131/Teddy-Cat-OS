#pragma once
#include <type.h>
#include <arch/riscv32/page.h>
#include <kernel/util.h>

#define TIME_SLICE 1000000 //100ms

extern struct task *current;

int create_process(uintptr_t func);

void init_schedule();

struct task *schedule();
