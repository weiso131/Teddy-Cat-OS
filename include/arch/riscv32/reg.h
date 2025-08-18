#define READ_REG(reg)                                                          \
    ({                                                                         \
        unsigned long __tmp;                                                   \
        __asm__ __volatile__("mv %0, " #reg : "=r"(__tmp));                  \
        __tmp;                                                                 \
    })
#define WRITE_REG(reg, value)                                                  \
    do {                                                                       \
        uint32_t __tmp = (value);                                              \
        __asm__ __volatile__("mv " #reg ", %0" ::"r"(__tmp));                \
    } while (0)
