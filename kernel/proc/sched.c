#include "sched.h"

#include "../hw/pic/pic.h"
#include "../hw/serial/serial.h"
#include "../panic/panic.h"

#include "../lib/dynarray.h"

dynarray *procs = {0};
uint32_t procs_head = 0;
proc *current_proc = 0;
uint8_t sched_initialzed = FALSE;

/*for debug & testing purposes only */
static uint32_t tick_count = 0;
volatile uint32_t proc1_counter = 0;
volatile uint32_t proc2_counter = 0;
/* ---- */

void scheduler_init(void) {
    procs = dynarray_new(SCHED_PROCS_POOL_INITIAL_SIZE);
    sched_initialzed = TRUE;
}

uint32_t scheduler(uint32_t *regs) {
    /*for debug & testing purposes only */
    tick_count++;
    if (tick_count % 1000 == 0) {
        serial_printf("tick %x p1=%x p2=%x\n", tick_count, proc1_counter, proc2_counter);
    }
    /* ---- */

    if (!sched_initialzed) kernel_panic("SCHEDULER_NOT_INITIALIZED: scheduler was called when it was not initialized!");
    if (procs->count == 0) {pic_sendEOI(0); return (uint32_t)regs; }

    if (current_proc != 0) {
        current_proc->kernel_stack = (uint32_t)regs;
    }
    procs_head = (procs_head + 1) % procs->count;
    proc *next = dynarray_get(procs, procs_head);
    current_proc = next;
    paging_switch(next->proc_pd);
    pic_sendEOI(0);
    return next->kernel_stack;
}

void scheduler_add_proc(proc *p) {
    if (!sched_initialzed) kernel_panic("SCHEDULER_NOT_INITIALIZED: tried to add a process to the schedular when it was not initialized!");
    if (!p) return;

    dynarray_add(procs, p);
}
