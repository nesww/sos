#ifndef KCORE_MEM
#define KCORE_MEM

#include "kcore.h"
#include "../mem/mem.h"

static inline void *kmalloc(uint32_t size) {
    return mem_heap_alloc(size);
}

static inline void kfree(void* ptr) {
    mem_heap_free(ptr);
}

#endif //KCORE_MEM
