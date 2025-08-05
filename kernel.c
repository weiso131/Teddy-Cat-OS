#include "util.h"

extern char __bss[], __bss_end[], __stack_top[];;



void kernel_main(void) {
    char test_array[17];

    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    memset(test_array, 'a', 16);
    printf("%s\n", test_array);
    for (;;) {
        long c = sbi_getchar();
        if (c < 0)
            continue;
        sbi_putchar(c);
    }
}

__attribute__((section(".text.boot")))
__attribute__((naked))
void boot()
{
    __asm__ __volatile__(
        "add sp, %[stack_top], x0\n"
        "j kernel_main\n"
        :
        : [stack_top] "r" (__stack_top)
    );
}
