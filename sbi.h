#pragma once

#include "type.h"

void sbi_putchar(char ch);

long sbi_getchar(void);

struct sbiret sbi_set_timer(uint64_t stime_value);
