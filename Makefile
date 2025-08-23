# ===== Compiler & Flags =====
CC      := clang
CFLAGS  := -std=c11 -O2 -g3 -Wall -Wextra -I ./include \
           --target=riscv32-unknown-elf \
           -fno-stack-protector -ffreestanding -nostdlib

LDFLAGS := -Wl,-Tkernel.ld -Wl,-Map=kernel.map
KERNEL  := kernel.elf

OBJCOPY := /usr/bin/llvm-objcopy

# ===== Sources =====
ARCH_SRCS   := arch/riscv32/boot.c \
               arch/riscv32/trap.c \
               arch/riscv32/page.c \
               arch/riscv32/sbi.c

KERNEL_SRCS := kernel/main.c kernel/sched.c kernel/util.c

APP_SRCS := user_app/test_app.c user/start.c
APP_ELF  := test_app.elf
APP_BIN  := test_app.bin
APP_OBJ  := test_app.bin.o

SRCS := $(ARCH_SRCS) $(KERNEL_SRCS)
OBJS := $(SRCS:.c=.o)

# ===== QEMU =====
QEMU      := qemu-system-riscv32
QEMUFLAGS := -machine virt -bios default -nographic -serial mon:stdio --no-reboot

# ===== Rules =====
.PHONY: all run clean

all: $(KERNEL)

# Build kernel (depends on app object)
$(KERNEL): $(OBJS) $(APP_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Build application ELF
$(APP_ELF): $(APP_SRCS)
	$(CC) $(CFLAGS) -Wl,-Tuser.ld -Wl,-Map=test_app.map -o $@ $^

# Convert ELF -> BIN
$(APP_BIN): $(APP_ELF)
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary $< $@

# Convert BIN -> relocatable object
$(APP_OBJ): $(APP_BIN)
	$(OBJCOPY) -Ibinary -Oelf32-littleriscv $< $@

# Generic object rule
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(KERNEL)
	$(QEMU) $(QEMUFLAGS) -kernel $(KERNEL)

clean:
	rm -f $(OBJS) $(KERNEL) kernel.map \
	      $(APP_ELF) $(APP_BIN) $(APP_OBJ) test_app.map
