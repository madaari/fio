#include "../arch/arch.h"

unsigned long arch_flags = 0;
#ifndef __rtems__
bool tsc_reliable;
#endif /* __rtems__ */
int arch_random;
