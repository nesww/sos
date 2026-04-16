#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_TAB_SIZE 256

void isr_handler(int num);
void idt_init(void);

// INTERRUPTS
#define INT_DIVZERO 0
#define INT_INVALID_OPCODE 6
#define INT_DOUBLE_FAULT 8
#define INT_GEN_PROTECT_FAULT 13
#define INT_PAGE_FAULT 14


struct interrupt_frame {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};

#define ISR(num) \
    __attribute__((interrupt)) void isr##num(struct interrupt_frame *frame) { \
        isr_handler(num); \
    }

typedef struct {
    uint16_t low_handler_addr;
    uint16_t segment_selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t high_handler_addr;
} __attribute__((packed)) idt_entry;


typedef struct {
   uint16_t limit;
   uint32_t base;
} __attribute__((packed))idt_descriptor;

#endif
