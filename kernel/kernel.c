#include "idt/idt.h"
#include "lib/kcore.h"
#include "pic/pic.h"
#include "vga/vga.h"
#include "mem/mem.h"
#include <stdint.h>
#include "lib/kmem.h"

static void __kernel_init(void) {
    vga_clear();
    vga_enable_cursor();
    pic_init();
    idt_init();
    mem_heap_init();
    INTERRUPTS_ENABLE();
    vga_printf("kernel started successfully!\n");
}

void kernel_main(void) {
    __kernel_init();

    uint32_t *mmap_entry_count = (uint32_t*)0x500;
    vga_printf("Memory map entries: %d\n", *mmap_entry_count);

    uint32_t* ptr = (uint32_t*)kmalloc(32);
    *ptr = 0xDEADBEEF;
    vga_printf("kmalloc test: %x\n", *ptr);

    void *x = kmalloc(MB(3)); //set it to 10 instead of 3 if you want panic for max heap allocation overflow
    // int a = 0/0; //uncomment if you want kernel panic with divzero & intframe
    kfree(ptr);
    kfree(x);

    while(1);
}
