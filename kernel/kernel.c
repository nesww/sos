#include "idt/idt.h"
#include "vga/vga.h"

void kernel_main(void) {
    vga_clear();
    idt_init();
    int a = 1/0; //test interrupt 0 => division by zero
    while(1);
}
