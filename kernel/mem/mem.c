#include "mem.h"
#include "../lib/kcore.h"
#include "../panic/panic.h"

static mem_heap heap = {0};

extern uint32_t kernel_end;

mmap_entry mmap_get_usable_entry(void) {
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

    mmap_entry entry = mmap_get_usable_entry();
    heap.heap_size = KMIN(entry.base_addr + entry.region_len - (uint32_t)heap.heap_start, MB(4));

    heap.initialized = TRUE;
}
void *mem_heap_alloc(uint32_t size) {
    if (!heap.initialized) kernel_panic("HEAP_NOT_INITIALIZED: tried to allocate before heap init!");
    if ((uint32_t)heap.heap_cursor+ size > heap.heap_size) {
        kernel_panic("HEAP_FULL: heap cannot allocate!");
    }
    uint32_t start = (uint32_t)heap.heap_start + (uint32_t)heap.heap_cursor;
    heap.heap_cursor = heap.heap_cursor + size;
    return (void*)start;
}

void mem_heap_free(void *ptr) {
    TODO("mem_heap_free");
}

void *mem_heap_get_start() {
    if (heap.initialized)
        return heap.heap_start;
    else {
        kernel_panic("Tried to access to heap start pointer with non-initialized heap");
        __builtin_unreachable();
    }
}

uint32_t mem_heap_get_size() {
    if (heap.initialized)
        return heap.heap_size;
    else {
        kernel_panic("Tried to access to heap size with non-initialized heap");
        __builtin_unreachable();
    }
}
