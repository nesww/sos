/*hardware related includes */
#include "hw/idt/idt.h"
#include "hw/pic/pic.h"
#include "hw/pit/pit.h"
#include "hw/serial/serial.h"
#include "hw/vga/vga.h"

/*kernel related includes */
#include "alloc/alloc.h"
#include "alloc/alloc.h"
#include "frame/frame.h"
#include "paging/paging.h"
#include "proc/proc.h"
#include "proc/sched.h"

/* libs includes */
#include <stdint.h>

#define SOS_VER_MAJOR 0
#define SOS_VER_MINOR 0
#define SOS_VER_PATCH 1

static void __kernel_init(void) {
    serial_init();
    vga_clear();
    vga_enable_cursor();
    pic_init();
    pit_init();
    idt_init();
    kheap_init();
    fa_init();
    paging_kernel_init();
    scheduler_init();
    INTERRUPTS_ENABLE();

    vga_printf("sOS - v%d.%d.%d\n", SOS_VER_MAJOR, SOS_VER_MINOR, SOS_VER_PATCH);
    vga_printf("kernel started successfully!\n");
}

//see proc/sched.c for usage
extern volatile uint32_t proc1_counter;
extern volatile uint32_t proc2_counter;

void __proc1(void) { serial_printf("proc1 started\n"); while(1) proc1_counter++; }
void __proc2(void) { serial_printf("proc2 started\n"); while(1) proc2_counter++; }


void kernel_main(void) {
    __kernel_init();

    proc *p_proc1 = proc_create(__proc1);
    proc *p_proc2 = proc_create(__proc2);

    scheduler_add_proc(p_proc1);
    scheduler_add_proc(p_proc2);


    while(1);
}
