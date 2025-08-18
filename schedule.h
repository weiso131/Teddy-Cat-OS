#pragma once
#include "type.h"
#include "page.h"
#include "util.h"

#define TIME_SLICE 1000000 //100ms

struct task {
    uint32_t sepc, sp;
    uint32_t *kernel_stack;
    uint32_t *page_table;
};

extern struct task *current;

int create_process(uintptr_t func);

void switch_context(struct task *current, struct task *next, uint32_t sp);

void init_schedule();

struct task *schedule();
