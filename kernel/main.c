#include <kernel/sched.h>
#include <kernel/util.h>
#include <type.h>

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
    create_process((uintptr_t)test_func1);
    create_process((uintptr_t)test_func2);
    
    printf("%x, %x\n", test_func1, test_func2);

    init_schedule();
}
