#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <stdint.h>

#include "proc.h"

#define SCHED_PROCS_POOL_INITIAL_SIZE 32

void scheduler_init(void);
void scheduler_add_proc(proc *p);
uint32_t scheduler(uint32_t *regs);


#endif //SCHEDULING_H
