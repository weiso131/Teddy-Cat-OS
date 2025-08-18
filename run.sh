#!/bin/bash
set -xue

QEMU=qemu-system-riscv32

CC=clang
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra -I ./include --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib"

$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    arch/riscv32/boot.c arch/riscv32/trap.c kernel/main.c kernel/sched.c arch/riscv32/page.c kernel/util.c arch/riscv32/sbi.c

$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -kernel kernel.elf
