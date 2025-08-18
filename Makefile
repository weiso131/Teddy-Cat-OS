# ===== Compiler & Flags =====
CC      := clang
CFLAGS  := -std=c11 -O2 -g3 -Wall -Wextra -I ./include \
           --target=riscv32-unknown-elf \
           -fno-stack-protector -ffreestanding -nostdlib

LDFLAGS := -Wl,-Tkernel.ld -Wl,-Map=kernel.map
KERNEL  := kernel.elf

# ===== Sources =====
ARCH_SRCS   := arch/riscv32/boot.c \
               arch/riscv32/trap.c \
               arch/riscv32/page.c \
               arch/riscv32/sbi.c

KERNEL_SRCS := kernel/main.c kernel/sched.c kernel/util.c

SRCS := $(ARCH_SRCS) $(KERNEL_SRCS)
OBJS := $(SRCS:.c=.o)

# ===== QEMU =====
QEMU      := qemu-system-riscv32
QEMUFLAGS := -machine virt -bios default -nographic -serial mon:stdio --no-reboot

# ===== Rules =====
.PHONY: all run clean

all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(KERNEL)
	$(QEMU) $(QEMUFLAGS) -kernel $(KERNEL)

clean:
	rm -f $(OBJS) $(KERNEL) kernel.map
