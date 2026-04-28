#ifndef KCORE_MEM
#define KCORE_MEM

#include "core.h"
#include "../alloc/alloc.h"

static inline void *kmalloc(uint32_t size) {
    return kheap_alloc(size);
}

static inline void kfree(void* ptr) {
    return kheap_free(ptr);
}

#endif //KCORE_MEM
