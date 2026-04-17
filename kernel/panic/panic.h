#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

#include "../vga/vga.h"
#include "../types.h"

static void kernel_panic(int int_num, struct interrupt_frame *int_frame) {
    vga_cclear(VGA_BLUE);
    vga_setbackground_color(VGA_BLUE);
    vga_printf("KERNEL PANIC\n> Interrupt: %d\n> State frame: \n>> IP (where): %x\n>> CPU flags: %x\n>> CS (code segment): %x\n>> SS (stack segment): %x\n>> SP (stack pointer): %x\n"
        , int_num, int_frame->ip, int_frame->flags, int_frame->cs, int_frame->ss, int_frame->sp);
    __asm__ volatile("hlt");
}

#endif //KERNEL_PANIC_H
