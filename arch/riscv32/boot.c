extern char __bss[], __bss_end[], __stack_top[];;

extern void kernel_main(void);

extern void kernel_entry(void);

__attribute__((section(".text.boot")))
__attribute__((naked))
void boot()
{   
    __asm__ __volatile__(

        //enable STIE
        "csrr t0, sie\n"
        "li t1, 0x20\n"
        "or t0, t0, t1\n"
        "csrw sie, t0\n"
        
        //setting sstatus SIE
        "csrr t0, sstatus\n"
        "li t1, 0x2\n"
        "or t0, t0, t1\n"
        "csrw sstatus, t0\n"

        //setting trap handler
        "la t0, kernel_entry\n"
        "csrw stvec, t0\n"

        //setting stack pointer
        "add sp, %[stack_top], x0\n"
        
        "j kernel_main\n"
        :
        : [stack_top] "r" (__stack_top)
    );
}
