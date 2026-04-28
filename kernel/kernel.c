#include "alloc/alloc.h"
#include "idt/idt.h"
#include "pic/pic.h"
#include "serial/serial.h"
#include "vga/vga.h"
#include "alloc/alloc.h"
#include "frame/frame.h"
#include "paging/paging.h"
#include <stdint.h>
#include "lib/stdmem.h"

#define SOS_VER_MAJOR 0
#define SOS_VER_MINOR 0
#define SOS_VER_PATCH 1

static void __kernel_init(void) {
    serial_init();
    vga_clear();
    vga_enable_cursor();
    pic_init();
    idt_init();
    kheap_init();
    fa_init();
    paging_kernel_init();
    INTERRUPTS_ENABLE();

    vga_printf("sOS - v%d.%d.%d\n", SOS_VER_MAJOR, SOS_VER_MINOR, SOS_VER_PATCH);
    vga_printf("kernel started successfully!\n");
}

void kernel_main(void) {
    __kernel_init();

    uint32_t *mmap_entry_count = (uint32_t*)0x500;
    vga_printf("Memory map entries: %d\n", *mmap_entry_count);

    uint32_t* ptr = (uint32_t*)kmalloc(32);
    *ptr = 0xDEADBEEF;
    vga_printf("kmalloc test: %x\n", *ptr);

    kfree(ptr);
    kfree(0); // will not page fault :)

    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    vga_printf("CR0: %x\n", cr0);


    //test for kernel heap memory allocator
    void *A = kmalloc(16);
    void *B = kmalloc(16);
    kfree(A);
    kfree(B);

    while(1);
}
