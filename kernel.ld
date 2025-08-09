ENTRY(boot)

SECTIONS {
    . = 0x80200000;

    .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    .rodata : ALIGN(4) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(4) {
        *(.data .data.*);
    }

    .bss : ALIGN(4) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);

        . = ALIGN(16);
        __heap_map_start = .;
        . += 2048; /* 64 * 1024 / 4 / 8  */
        __heap_map_end = .;

        __bss_end = .;
    }

    . = ALIGN(4);
    . += 128 * 1024; /* 128KB */
    __stack_top = .;

    . = ALIGN(4096);
    __heap_start = .;
    . += 64 * 1024 * 1024; /* 64MB */
    __heap_end = .;

    
}
