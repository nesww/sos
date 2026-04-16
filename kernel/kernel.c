#include "idt/idt.h"
#include "pic/pic.h"
#include "vga/vga.h"

void kernel_main(void) {
    vga_clear();
    pic_init();
    idt_init();
    __asm__ volatile("sti");
    while(1);
}
