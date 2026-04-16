#include "idt.h"
#include <stdint.h>
#include "../vga/vga.h"
static idt_entry idt[IDT_TAB_SIZE];

ISR(0)
ISR(1);
ISR(2);
ISR(3);
ISR(4);
ISR(5);
ISR(6);
ISR(7);
ISR(8);
ISR(9);
ISR(10);
ISR(11);
ISR(12);
ISR(13);
ISR(14);
ISR(15);
ISR(16);
ISR(17);
ISR(18);
ISR(19);
ISR(20);
ISR(21);
ISR(22);
ISR(23);
ISR(24);
ISR(25);
ISR(26);
ISR(27);
ISR(28);
ISR(29);
ISR(30);
ISR(31);

void _idt_set_entry(int num, void(*isr_wrapper)(struct interrupt_frame *)) {
    idt_entry entry = {0};
    entry.zero = 0x0;
    entry.segment_selector = 0x08;
    entry.type_attr = 0x8E;
    uint32_t addr = (uint32_t)isr_wrapper;
    entry.low_handler_addr = (uint16_t)(addr & 0xffff);
    entry.high_handler_addr = (uint16_t)(addr >> 16);
    idt[num] = entry;
}

void idt_init(void) {
    void(*handlers[])(struct interrupt_frame *) = {
        isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10,
        isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20,
        isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
    };

    for (int i = 0; i < 32; ++i) {
        _idt_set_entry(i, handlers[i]);
    }

    idt_descriptor desc;
    desc.limit = sizeof(idt) - 1;
    desc.base  = (uint32_t)idt;

    __asm__ volatile(
        "lidt %0" : : "m"(desc)
    );
}

void isr_handler(int num) {
    switch(num) {
        case INT_DIVZERO: vga_print("INT: division by zero"); break;
        case INT_INVALID_OPCODE: vga_print("INT: invalid opcode"); break;
        case INT_DOUBLE_FAULT: vga_print("INT: double fault"); break;
        case INT_GEN_PROTECT_FAULT: vga_print("INT: generation protection fault"); break;
        case INT_PAGE_FAULT: vga_print("INT: page fault"); break;
        default: vga_print("INT: unknown exception "); vga_putint(num); break;
    }
    vga_print("   /!\\ halting");
    __asm__ volatile("hlt");
}
