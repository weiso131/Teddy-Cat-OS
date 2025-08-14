#include "schedule.h"


struct task *current;

#define PROCESS_MAX 32

uint32_t process_use = 0;
struct task process_list[PROCESS_MAX];//this need to be replace by tree in future

uint32_t schedule_cnt = 0;


void init_schedule()
{
    current = schedule();
    timer_wait(TIME_SLICE);

    __asm__ __volatile__(
        "mv sp, %0\n"
        "mv ra, %1\n"
        "ret\n"
        :
        : "r" (current->kernel_stack + (1 << 12)), "r" (current->sepc)
        : 
    );
}

struct task *schedule()
{
    if (!process_use) {
        printf("idle\n");
        return 0;
    }

    for (int i = 0;i < PROCESS_MAX;i++) {
        
        schedule_cnt = (schedule_cnt + 1) & 0x1F;
        if ((process_use >> schedule_cnt) & 1)
            break;
    }
    return &process_list[schedule_cnt];

}

void switch_context(struct task *current, struct task *next) 
{
    __asm__ __volatile__(
        "csrr t0, sepc\n"
        "mv %0, t0\n"
        "mv %1, sp\n"
        "csrw sepc, %2\n"
        "mv sp, %3\n"
        : "=&r" (current->sepc), "=&r" (current->sp)
        : "r" (next->sepc), "r" (next->sp)
        : "t0", "memory"
    );    
}


int create_process(uintptr_t func)
{
    if (process_use == 0xFFFFFFFF) {
        printf("process list is full\n");
        return -1;
    }

    struct task *task = 0;
    for (int i = 0;i < PROCESS_MAX;i++) {
        if (!((process_use >> i) & 1)) {
            process_use |= 1 << i;
            task = process_list + i;
            break;
        }
    }

    task->kernel_stack = (uint32_t*)alloc_page();//this will be replace in virtual memory future
    task->sepc = func;
    task->sp = (uintptr_t)task->kernel_stack + (1 << 12) - 4 * 31 - 16;//16 is stack pointer offset of handle_trap
    task->kernel_stack[1023] = (uintptr_t)task->kernel_stack + (1 << 12);
    return 0;
}
