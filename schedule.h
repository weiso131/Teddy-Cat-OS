#pragma once
#include "type.h"
#include "page.h"
#include "util.h"

#define TIME_SLICE 1000000 //100ms

struct task {
    uint32_t sepc, sp;
    uint32_t *kernel_stack;
};

extern struct task *current;

int create_process(uintptr_t func);

void switch_context(struct task *current, struct task *next);

void init_schedule();

struct task *schedule();
