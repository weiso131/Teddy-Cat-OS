#pragma once

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

#if defined(__x86_64__) || defined(__amd64__) \
 || defined(__aarch64__) \
 || (defined(__riscv) && __riscv_xlen == 64)
    #define SYSTEM_ARCH_BITS 64
#else
    #define SYSTEM_ARCH_BITS 32

#endif
#define SYSTEM_ARCH_BYTES SYSTEM_ARCH_BITS / 8
typedef unsigned long uintptr_t;
typedef long intptr_t;

struct sbiret {
    long error;
    long value;
};
