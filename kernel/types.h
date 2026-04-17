#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <stdint.h>

struct interrupt_frame {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};

#endif //KERNEL_TYPES_H
