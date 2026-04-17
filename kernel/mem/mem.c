#include "mem.h"
#define KLIB_UTILS_IMPLEM
#include "../lib/kcore.h"
#include "../panic/panic.h"

static mem_heap heap = {0};

extern uint32_t kernel_end;

static mmap_entry __mmap_get_usable_entry(void) {
    mmap_entry *entries = mem_get_mmap_entries();
    for (uint32_t i = 0; i < *(uint32_t*)MMAP_COUNTER_ADDR; ++i) {
        mmap_entry entry = entries[i];
        if (entry.region_type == MMAP_REGTYPE_USABLE && entry.base_addr > 0x00100000) {
            return entry;
        }
    }
    return (mmap_entry){0};
}

void mem_heap_init(void) {
    if (heap.initialized) return;

    heap.heap_start =(void*)&kernel_end; /*the heap will start right after the kernel in memory */
    heap.heap_cursor = 0;

    mmap_entry entry = __mmap_get_usable_entry();
    heap.heap_size = KMIN(entry.base_addr + entry.region_len - (uint32_t)heap.heap_start, MB(4));

    heap.initialized = TRUE;
}
void *mem_heap_alloc(uint32_t size) {
    if ((uint32_t)heap.heap_cursor+ size > heap.heap_size) {
        kernel_panic("HEAP_FULL: heap cannot allocate");
    }
    uint32_t start = (uint32_t)heap.heap_start + (uint32_t)heap.heap_cursor;
    heap.heap_cursor = heap.heap_cursor + size;
    return (void*)start;
}

void mem_heap_free(void *ptr) {
    TODO("mem_heap_free");
}
