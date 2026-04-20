#include "idt/idt.h"
#include "lib/kcore.h"
#include "pic/pic.h"
#include "vga/vga.h"
#include "mem/mem.h"
#include "frame/frame.h"
#include "paging/paging.h"
#include <stdint.h>
#include "lib/kmem.h"

static void __kernel_init(void) {
    vga_clear();
    vga_enable_cursor();
    pic_init();
    idt_init();
    mem_heap_init();
    fa_init();
    paging_init();
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

    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    vga_printf("CR0: %x\n", cr0);

    //triggering a page fault for panicking on access to non-paged memory address for kernel page directory
    uint32_t *not_cool = (uint32_t*)0xDEAD0000;
    *not_cool = 0;

    //will never be reach, for now
    void *x = kmalloc(MB(3)); //set it to 10 instead of 3 if you want panic for max heap allocation overflow
    // int a = 0/0; //uncomment if you want kernel panic with divzero & intframe
    kfree(ptr);
    kfree(x);

    while(1);
}
