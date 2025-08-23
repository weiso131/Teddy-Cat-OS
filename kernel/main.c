#include <kernel/sched.h>
#include <kernel/util.h>
#include <type.h>

extern char _binary_test_app_bin_start[], _binary_test_app_bin_size[];

void test_func1()
{
    while(1) {
        printf("task 1\n");
        __asm__ __volatile__("wfi");
    }
}

void test_func2()
{
    while(1) {
        printf("task 2\n");
        __asm__ __volatile__("wfi");
    }  
}

void kernel_main(void)
{
    create_kernel_process((uintptr_t)test_func1);
    create_kernel_process((uintptr_t)test_func2);
    create_user_process(_binary_test_app_bin_start, _binary_test_app_bin_size);

    init_schedule();
}
