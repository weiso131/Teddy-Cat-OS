#include <kernel/sched.h>
#include <arch/riscv32/timer.h>
#include <arch/riscv32/context.h>
#include <kernel/util.h>
#include <type.h>

extern char __kernel_base[], __heap_end[];
struct task *current;

#define PROCESS_MAX 32
#define USER_BASE 0x1000000

uint32_t process_use = 0;
struct task process_list[PROCESS_MAX];//this need to be replace by tree in future

uint32_t schedule_cnt = 0;


void init_schedule()
{
    current = schedule();
    timer_wait(TIME_SLICE);
    start_task(current);
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

int create_process(uintptr_t func, size_t size)
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

    // Map kernel pages.
    uint32_t *page_table = (uint32_t *) alloc_page();
    for (uintptr_t paddr = (uintptr_t) __kernel_base;
         paddr < (uintptr_t) __heap_end; paddr += PAGE_SIZE)
        map_vpage(page_table, paddr, paddr, PAGE_R | PAGE_W | PAGE_X);

    task->page_table = page_table;

    // Map user pages.
    for (uintptr_t vaddr = 0;
         vaddr < (uintptr_t) size; vaddr += PAGE_SIZE) {
        char *page = alloc_page();
        memcpy(page, (uint32_t *)(func + vaddr), min(PAGE_SIZE, size - vaddr));
        map_vpage(page_table, USER_BASE + vaddr, (uint32_t)page, PAGE_R | PAGE_W | PAGE_X | PAGE_U);
    }

    task->kernel_stack = (uint32_t*)alloc_page();//this will be replace in virtual memory future
    task->sepc = func;
    task->sp = (uintptr_t)task->kernel_stack + (1 << 12) - 4 * 31;
    task->kernel_stack[1023] = (uintptr_t)task->kernel_stack + (1 << 12);

    if (size) {
        task->user = 1;
        task->sepc = USER_BASE;
    }
    return 0;
}
