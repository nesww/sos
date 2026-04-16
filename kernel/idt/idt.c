#include "idt.h"
#include <stdint.h>
#include "../vga/vga.h"
#include "../pic/pic.h"
static idt_entry idt[IDT_TAB_SIZE];

//intel legacy ISRs
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

// PIC ISRs
ISR(32);
ISR(33);
ISR(34);
ISR(35);
ISR(36);
ISR(37);
ISR(38);
ISR(39);
ISR(40);
ISR(41);
ISR(42);
ISR(43);
ISR(44);
ISR(45);
ISR(46);
ISR(47);

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
        // intel legacy ISRs
        isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10,
        isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20,
        isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31,
        // PIC ISRs
        isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39, isr40, isr41, isr42, isr43, isr44, isr45, isr46, isr47
    };

    for (int i = 0; i < 48; ++i) {
        _idt_set_entry(i, handlers[i]);
    }

    // idt_descriptor desc;
    // desc.limit = sizeof(idt) - 1;
    // desc.base  = (uint32_t)idt;
    uint8_t idt_ptr[6];
    *(uint16_t *)idt_ptr = sizeof(idt) - 1;
    *(uint32_t *)(idt_ptr + 2) = (uint32_t)idt;

    __asm__ volatile(
        "lidt %0" : : "m"(*idt_ptr)
    );
}

void isr_handler(int num) {
    if (num < 32) {
        switch(num) {
            //legacy ints
            case INT_DIVZERO:           vga_print("INT: division by zero");            break;
            case INT_INVALID_OPCODE:    vga_print("INT: invalid opcode");              break;
            case INT_DOUBLE_FAULT:      vga_print("INT: double fault");                break;
            case INT_GEN_PROTECT_FAULT: vga_print("INT: generation protection fault"); break;
            case INT_PAGE_FAULT:        vga_print("INT: page fault");                  break;
            default: vga_print("INT: unknown exception or not handled"); vga_putint(num);            break;

        }
        vga_print("   /!\\ halting");
        __asm__ volatile("hlt");
    } else {
        switch(num) {
            //PIC ints
            // PIC master IRQs
            case INT_PIC_TIMER:          /*vga_print("INT_PIC: timer");*/              break;
            case INT_PIC_KEYBOARD:
                vga_print("INT_PIC: keyboard, scancode: ");
                uint8_t scancode = inb(IO_KEYBOARD_DATA_PORT);
                vga_puthex(scancode);
                vga_println("");
                break;
            case INT_PIC_SERIAL_COM2:       vga_print("INT_PIC: serial COM2");         break;
            case INT_PIC_SERIAL_COM1:       vga_print("INT_PIC: serial COM1");         break;
            case INT_PIC_PARALLEL_PORT:     vga_print("INT_PIC: parallel port");       break;
            case INT_PIC_FLOPPY:            vga_print("INT_PIC: floppy");              break;
            case INT_PIC_PARALLEL_PORT2:    vga_print("INT_PIC: parallel port 2");     break;
            // PIC slave IRQs
            case INT_PIC_REALTIME_CLOCK:    vga_print("INT_PIC: realtime clock");      break;
            case INT_PIC_PS2:               vga_print("INT_PIC: PS/2");                break;
            case INT_PIC_FLOAT_COPROCESSOR: vga_print("INT_PIC: float coprocessor");   break;
            case INT_PIC_ATA_PRIMARY:       vga_print("INT_PIC: ATA primary disk");    break;
            case INT_PIC_ATA_SECONDARY:     vga_print("INT_PIC: ATA secondary disk");  break;
            default:
                vga_print("INT_PIC: unknown exception or not handled ");
                vga_putint(num);
                __vga_new_line();
                break;
        }
        pic_sendEOI(num - PIC_INT_OFFSET);
    }
}
