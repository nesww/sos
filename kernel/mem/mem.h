#ifndef KERNEL_MEM_H
#define KERNEL_MEM_H

#include <stdint.h>

#define MMAP_ENTRY_START_ADDR 0x504
#define MMAP_COUNTER_ADDR     0x500

#define MMAP_REGTYPE_USABLE   1
#define MMAP_REGTYPE_RESERVED 2


typedef struct {
    uint64_t base_addr;
    uint64_t region_len;
    uint32_t region_type;
    uint32_t __reserved;
}__attribute__((packed)) mmap_entry;

typedef struct {
    void     *heap_start;    /*pointer to the start of the heap */
    uint32_t heap_cursor;    /*pointer to the next writable byte in the heap */
    uint32_t heap_size;      /*maximum allowed size of the heap */
    uint8_t  initialized;    /*if the heap was initialized */
} __attribute__((packed)) mem_heap;

static inline mmap_entry *mem_get_mmap_entries(void) {
    return (mmap_entry*)0x504;
}

void mem_heap_init(void);
void *mem_heap_alloc(uint32_t size);
void mem_heap_free(void *ptr);

#endif //KERNEL_MEM_H
